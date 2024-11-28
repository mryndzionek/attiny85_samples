#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/power.h>

#include <util/delay.h>
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

static inline uint16_t rand()
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

static void candle(void)
{
    static uint8_t g_cfg;
    // OCA&OCB PWM enabled; fast PWM mode 3
    TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(COM0B0) | _BV(WGM01) | _BV(WGM00);
    TCCR0B = _BV(CS00); // CK/1

    void out1(uint8_t v)
    {
        OCR0A = 0xff - v;
    }

    void out2(uint8_t v)
    {
        OCR0B = v;
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

    while (1)
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
            // PB3=1 -> mode 1

            // flame position
            posr = ((r >> 6) & 0x1ff) - 0x80;
            // bandpass filter
            posad += posr - pos;
            pos += posad / 16;
            pos -= (pos - 0x80) / 8;
        }
        else
        {
            // PB3=0 -> mode 0

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

        // wait for sync
        while (g_tcnt)
            asm volatile("sleep");
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

int main(void)
{
    // init:
    DDRB = PORTBMASK;
    PORTB = ((uint8_t)~PORTBMASK);

    // system clock prescaler:
    CLKPR = _BV(CLKPCE);
    CLKPR = 6; // osc/64 15.625kHz

    TCCR0B = _BV(CS00); // clk/1

    // OVF interrupt
    g_tcnt = 0;
    TIMSK = _BV(TOIE0);
    asm volatile("sei");
    // enable sleep
    MCUCR = _BV(SE);

    // disable comparator:
    SBI(ACSR, ACD);
    // shut down ADC:
    PRR = _BV(PRADC);

    g_tcnt = 16;
    // cfg(TRUE);
    candle();

    while (1)
        ;
}