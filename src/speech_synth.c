#include "speech_synth.h"

#include <stddef.h>
#include <stdbool.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include <util/atomic.h>

#include "speech_data.h"

#define SAMPLE_TO_PWM(_s) ((_s + (32767 / 2)) / (128))

struct _speech_synth_t
{
    fix16_t *g_addr;
    uint8_t *ps_addr;
    fix16_t *a_addr;
    uint16_t len;
    uint16_t frame_idx;

    uint16_t t;
    uint8_t ps;
    fix16_t g;
    uint16_t xs_idx;
    fix16_t xs[LPC_ORDER];
    fix16_t hs[LPC_ORDER];
};

// clang-format off
const fix16_t RND[256] PROGMEM = {
    494, 724, -351, 36, 87, 438, -35, 210, 389, 1017, -246, 803, 192, 329, -297, -364,
    -201, 707, -712, -938, -744, -944, 129, 1021, -998, 695, 798, 130, 514, -202, -341, -969,
    381, -295, 101, 809, -244, -46, -81, -149, 354, -232, 219, 735, 778, -532, 301, 0,
    503, -128, 244, 685, -245, -299, 6, -922, 956, -625, -383, 469, -935, -600, 704, -492,
    804, -346, 801, -802, 750, 644, 307, -362, -137, 949, 677, -851, 718, -1009, 172, -1017,
    472, -451, 4, 877, -639, 337, -335, -299, -684, 319, 391, -833, -405, 601, -126, 718,
    -860, -963, -988, -650, -309, -431, 116, 5, -769, -397, -427, -533, -346, -136, -644, 831,
    635, 75, -492, -779, 697, 270, -871, 1022, 232, -161, -1006, -808, -372, -301, 456, 1021,
    -50, -685, 628, 540, 235, -61, -747, -369, -998, 673, -676, 519, 369, 785, -332, 739,
    -505, 504, -113, -766, -96, 678, 37, 103, -786, 524, 920, 701, 297, 387, -318, -115,
    -92, 681, -732, 522, -626, -447, -610, -752, -641, -811, 967, -252, 342, -958, 992, 225,
    -859, -433, 346, 20, -932, -551, -51, 531, -500, -849, 491, -397, -276, 315, 500, -96,
    -98, -946, 838, 436, -751, -736, -558, 239, -430, -171, -412, 655, -705, 376, -574, 308,
    708, -772, 606, 951, -17, -1, 29, 575, -732, -831, 817, -703, 265, 927, -676, 708,
    -584, -339, -143, 10, -223, -224, -285, -669, 78, -998, -87, 743, 145, -825, 42, 930,
    -714, 52, -308, -841, 481, -382, 51, -127, 190, 65, 80, 147, 63, 59, -392, 871
};

// clang-format on

static volatile bool g_finished = false;
static volatile uint8_t buf_idx = 0;
static uint8_t samp_buf[2][LPC_FRAME_LEN];

ISR(TIMER0_COMPA_vect)
{
    static uint16_t idx = 0;
    static uint16_t sample = 0;

    sample = samp_buf[buf_idx][idx++];
    OCR1A = sample;
    OCR1B = sample ^ 255;

    if (idx == LPC_FRAME_LEN)
    {
        idx = 0;
        g_finished = true;
    }
}

static void update(speech_synth_t *self)
{
    self->xs_idx = 0;
    self->g = i16_to_fix(pgm_read_word(&self->g_addr[self->frame_idx]));
    self->ps = pgm_read_byte(&self->ps_addr[self->frame_idx]);

    for (uint8_t j = 0; j < LPC_ORDER; j++)
    {
        self->hs[j] = i16_to_fix(pgm_read_word(&self->a_addr[(self->frame_idx * LPC_ORDER) + j]));
        self->xs[j] = FIX_ZERO;
    }
    self->frame_idx++;
}

static void exec(speech_synth_t *self, uint8_t *y)
{
    static uint16_t rnd_idx = 0;
    static size_t last_p = 0;
    static fix16_t de_y_ = 0;
    int32_t x;

    for (uint16_t i = 0; i < LPC_FRAME_LEN; i++)
    {
        fix16_t noise = pgm_read_word(&RND[rnd_idx]);
        rnd_idx = (rnd_idx + 1) % 256;

        if (self->ps < 0x15)
        {
            x = fix_mul(self->g, noise);
        }
        else
        {
            if ((self->t - last_p) >= (self->ps))
            {
                last_p = self->t;
                x = 4 * self->g;
            }
            else
            {
                x = 0;
            }
        }

        for (uint16_t k = 0; k < LPC_ORDER; k++)
        {
            x += fix_mul(self->hs[k], self->xs[k]);
        }

        fix16_t de_y = x + fix_mul(LPC_DEEMPHASIS_FACTOR, de_y_);
        de_y_ = de_y;

        for (uint16_t i = LPC_ORDER - 1; i > 0; i--)
        {
            self->xs[i] = self->xs[i - 1];
        }

        self->xs[0] = x;
        self->xs_idx = (self->xs_idx + 1) % LPC_ORDER;
        y[i] = SAMPLE_TO_PWM(de_y);
        self->t++;
    }
}

speech_synth_t *speech_synth_init(void)
{
    static speech_synth_t mem[1];
    static uint16_t mem_idx = 0;

    if (mem_idx > 0)
    {
        return NULL;
    }

    speech_synth_t *self = &mem[mem_idx++];

    {
        PLLCSR = _BV(PCKE) | _BV(PLLE);

        // Set up Timer/Counter1 for PWM output
        TIMSK = 0;                                    // Timer interrupts OFF
        TCCR1 = 1 << PWM1A | 2 << COM1A0 | 1 << CS11; // PWM A, clear on match, 1:2 prescale
        GTCCR = 1 << PWM1B | 2 << COM1B0;             // PWM B, clear on match
        OCR1A = 0;
        OCR1B = 0;

        // DDRB |= 1 << PB3;
        // PORTB &= ~_BV(PB3);

        TCCR0A = 3 << WGM00;             // Fast PWM
        TCCR0B = 1 << WGM02 | 2 << CS00; // 1/8 prescale
        TIMSK = 1 << OCIE0A;             // Enable compare match
        OCR0A = 250 - 1;                 // 4kHz

        DDRB |= _BV(PB4) | _BV(PB1);
        TIMSK &= ~_BV(OCIE0A);
    }

    // Turn on timer
    TIMSK |= _BV(OCIE0A);
    return self;
}

void speech_synth_say(speech_synth_t *self, lpc_name_e phrase_id)
{
    bool finished = false;

    if (phrase_id >= lpc_name_max)
    {
        return;
    }

    self->t = 0;
    self->frame_idx = 0;
    self->g_addr = (fix16_t *)pgm_read_word(&LPC_G_ADDRS[phrase_id]);
    self->ps_addr = (uint8_t *)pgm_read_word(&LPC_PS_ADDRS[phrase_id]);
    self->a_addr = (fix16_t *)pgm_read_word(&LPC_A_ADDRS[phrase_id]);
    self->len = pgm_read_word(&LPC_LENGTHS[phrase_id]);

    update(self);
    exec(self, samp_buf[buf_idx]);

    for (;;)
    {
        update(self);
        if (self->frame_idx == self->len)
        {
            break;
        }

        exec(self, samp_buf[buf_idx ^ 1]);

        do
        {
            sleep_mode();
            ATOMIC_BLOCK(ATOMIC_FORCEON)
            {
                finished = g_finished;
                g_finished = false;
            }
        } while (!finished);
        buf_idx ^= 1;
    }
}
