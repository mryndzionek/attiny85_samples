#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/power.h>

#include <util/atomic.h>

#include <stdbool.h>

#define PORTBMASK (_BV(PB_LED1) | _BV(PB_LED2))
#define SBI(port, val) asm volatile("sbi %0,%1" ::"I"(&(port) - 32), "I"(val))

static volatile uint8_t g_tcnt;

// PORTB:
enum
{
    PB_LED1 = 0,
    PB_LED2 = 1
};

ISR(TIMER0_OVF_vect)
{
    if (g_tcnt)
        g_tcnt--;
}

ISR(WDT_vect)
{
}

static inline uint16_t rand(void)
{
    static uint16_t g_lfsr = 1;
#define RNDMASK 0xB400u

    uint8_t lbs = g_lfsr;
    g_lfsr = (g_lfsr >> 1);
    if (lbs & 1)
        g_lfsr ^= RNDMASK;
    return g_lfsr;
}

enum
{
    CFG_MODE = 0x80,
    CFG_DMAX = 2
};

static bool is_dark(void)
{
    static bool is_dark = false;
    static uint32_t adc_val = 0;
    static uint8_t adc_count = 0;

    ADCSRA |= _BV(ADSC);
    while (ADCSRA & _BV(ADSC))
        ;

    uint16_t adc_l = ADCL;
    adc_val += (ADCH << 8) | adc_l;

    if (++adc_count == 16)
    {
        adc_count = 0;
        adc_val /= 16;

        switch (is_dark)
        {
        case true:
            if (adc_val >= 110)
            {
                is_dark = false;
            }
            break;

        case false:
            if (adc_val <= 70)
            {
                is_dark = true;
            }
            break;
        }
        adc_val = 0;
    }

    return is_dark;
}

static void deep_sleep(uint8_t i)
{
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
        ADCSRA &= ~_BV(ADEN);
        power_all_disable();
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    }

    while (i--)
    {
        MCUSR &= ~_BV(WDRF);
        WDTCR |= (_BV(WDCE) | _BV(WDE));
        WDTCR = _BV(WDP2) | _BV(WDP0);
        WDTCR |= _BV(WDIE);
        sleep_mode();
    }

    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
        power_all_enable();
        set_sleep_mode(SLEEP_MODE_IDLE);
        ADCSRA |= _BV(ADEN);
    }
}

static void candle(void)
{
    static uint8_t g_cfg;
    // OCA&OCB PWM enabled; fast PWM mode 3
    TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);
    TCCR0B = _BV(CS00); // CK/1

    void out1(uint8_t v)
    {
        OCR0A = 0xff - v;
    }

    void out2(uint8_t v)
    {
        OCR0B = 0xff - v;
    }

    void cfgup(void)
    {
        if ((++g_cfg & 3) > CFG_DMAX)
        {
            g_cfg ^= CFG_MODE;
            g_cfg &= ~3;
        }
    }

    out1(0);
    out2(0);

    // rnd matcher
    uint8_t rswitch = 0;

    // LED state:
    int16_t pos = 0;
    int16_t posad = 0;
    int16_t pow = 0;
    int16_t powad = 0;

    while (true)
    {
        enum
        {
            POWMID = 0xb0
        };
        int16_t powr, posr;
        uint16_t r;
        uint16_t pow2;

        r = rand();
        if (r == 1)
            rswitch++;

        if ((r & 0xff) == rswitch)
            cfgup();

        // flame power
        powr = ((r >> 2) & 0x1f) + POWMID - 0x0f;
        // state variable bandpass filter
        powad += powr - pow;
        pow += powad / 4;
        pow -= (pow - POWMID) / 8;

        if (g_cfg & CFG_MODE)
        {
            // flame position
            posr = ((r >> 6) & 0x1ff) - 0x80;
            // bandpass filter
            posad += posr - pos;
            pos += posad / 16;
            pos -= (pos - 0x80) / 8;
        }
        else
        {
            // flame position
            posr = ((r >> 6) & 0x7f) + 0x40;
            // bandpass filter
            posad += posr - pos;
            pos += posad / 8;
            pos -= (pos - 0x80) / 64;
        }

        enum
        {
            POWMIN = 0x40,
            POWMAX = 0xe0
        };
        if (pow < POWMIN)
            pow = POWMIN;
        if (pow > POWMAX)
            pow = POWMAX;

        enum
        {
            POSMIN = 0x10,
            POSMAX = 0xf0
        };
        if (pos < POSMIN)
            pos = POSMIN;
        if (pos > POSMAX)
            pos = POSMAX;

        pow2 = ((uint16_t)pow * (uint16_t)pow) >> 8;

        if (!is_dark())
        {
            out1(0);
            out2(0);
            PORTB &= (~(uint8_t)PORTBMASK);
            do
            {
                deep_sleep(1);
            } while (!is_dark());
        }

        // wait for sync
        while (g_tcnt)
        {
            sleep_mode();
        }

        // set delay
        switch (g_cfg & 3)
        {
#if 1
        case 0:
            g_tcnt = 16;
            break;
        case 1:
            g_tcnt = 18;
            break;
        default:
            g_tcnt = 20;
            break;
#else
        case 0:
            g_tcnt = 20;
            break;
        case 1:
            g_tcnt = 30;
            break;
        default:
            g_tcnt = 40;
            break;
#endif
        }

        out1(((uint16_t)pos * pow2) >> 8);
        out2(((uint16_t)(0xff - pos) * pow2) >> 8);
    }
}

static void adc_init(void)
{
    PORTB &= ~_BV(PB2); // Disable pull-up
    ADCSRA |= _BV(ADEN);
    ADMUX = _BV(0); // configuring PB2 to take input
}

int main(void)
{
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
        MCUSR &= ~_BV(WDRF);
    }

    // init:
    DDRB = PORTBMASK;
    PORTB &= (~(uint8_t)PORTBMASK);

    // system clock prescaler:
    CLKPR = _BV(CLKPCE);
    CLKPR = 6; // osc/64 15.625kHz

    TCCR0B = _BV(CS00); // clk/1

    // OVF interrupt
    g_tcnt = 0;
    TIMSK = _BV(TOIE0);

    adc_init();
    sei();

    // disable comparator:
    SBI(ACSR, ACD);

    g_tcnt = 16;
    candle();
}