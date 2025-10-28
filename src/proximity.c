#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/power.h>

#include <util/atomic.h>

#include <stdbool.h>

#define NUM_TAPS (21)

// clang-format off
static const int16_t taps[NUM_TAPS] = {
    -24, 129, 0, -437, 202, -1834, -1776, -610,
    -8107, 3302, 20445, 3302, -8107, -610, -1776, -1834,
    202, -437, 0, 129, -24,
};
// clang-format on

ISR(WDT_vect)
{
}

__attribute__((always_inline)) int16_t fix_mul(int16_t x, int16_t y)
{
    int32_t res = (int32_t)x * y;
    return (int16_t)(res >> 15);
}

static int16_t filter(int16_t x)
{
    static int16_t xs[NUM_TAPS];
    int32_t accum = 0;

    for (uint8_t i = NUM_TAPS - 1; i > 0; i--)
    {
        xs[i] = xs[i - 1];
    }

    xs[0] = x;

    for (uint8_t i = 0; i < NUM_TAPS; i++)
    {
        if (taps[i] != 0)
        {
            accum += fix_mul(xs[i], taps[i]);
        }
    }

    return accum;
}

static void adc_init(void)
{
    PORTB &= ~_BV(PB2); // Disable pull-up
    ADCSRA |= _BV(ADEN);
    ADMUX = _BV(0); // configuring PB2 to take input
}

static inline uint16_t adc_read(void)
{
    ADCSRA |= _BV(ADSC);
    while (ADCSRA & _BV(ADSC))
        ;

    uint16_t adc_l = ADCL;
    uint32_t adc_val = (ADCH << 8) | adc_l;

    return adc_val;
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
        WDTCR = _BV(WDP1) | _BV(WDP0); // 0.125s - 8Hz
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

int main(void)
{
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
        MCUSR &= ~_BV(WDRF);
    }

    DDRB |= _BV(PB3);
    PORTB &= ~_BV(PB3);
    adc_init();
    sei();

    while (true)
    {
        deep_sleep(1);
        int16_t x = adc_read();
        int16_t y = filter(x);

        if (y < -110)
        {
            PORTB |= _BV(PB3);
        }
        else
        {
            PORTB &= ~_BV(PB3);
        }
    }
}
