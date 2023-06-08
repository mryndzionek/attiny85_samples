#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <avr/power.h>
#include <util/atomic.h>

#include <stdbool.h>

#include "snd/snd.h"

#define USE_BH1750

#ifdef USE_BH1750
#include "i2c_master_usi.h"

#define BH1750_ADDR (0x23)
#define ONE_TIME_HIGH_RES_MODE (0x20)
#define ONE_TIME_HIGH_RES_MODE_2 (0x21)
#define ONE_TIME_LOW_RES_MODE (0x23)
#endif

#ifndef USE_BH1750
static void adc_init(void)
{
    ADCSRA |= _BV(ADEN);
    ADMUX = _BV(0); // configuring PB2 to take input
    ADCSRB = 0x00;  // Configuring free running mode
    ADCSRA |= (1 << ADSC) | (1 << ADATE);
}
#endif

static void setup(void)
{
    // Enable 64 MHz PLL and use as source for Timer1
    PLLCSR = _BV(PCKE) | _BV(PLLE);

    // Set up Timer/Counter1 for PWM output
    TIMSK = 0;                                    // Timer interrupts OFF
    TCCR1 = 1 << PWM1A | 2 << COM1A0 | 1 << CS10; // PWM A, clear on match, 1:1 prescale
    GTCCR = 1 << PWM1B | 2 << COM1B0;             // PWM B, clear on match
    OCR1A = 0;
    OCR1B = 0;

    DDRB |= 1 << PB3;
    PORTB &= ~_BV(PB3);

    TCCR0A = 3 << WGM00;             // Fast PWM
    TCCR0B = 1 << WGM02 | 2 << CS00; // 1/8 prescale
    TIMSK = 1 << OCIE0A;             // Enable compare match
    OCR0A = SND_DIV - 1;

    DDRB |= _BV(PB4) | _BV(PB1);
    TIMSK &= ~_BV(OCIE0A);

#ifdef USE_BH1750
    i2c_init();
#else
    adc_init();
#endif
}

static volatile bool g_finished = false;
ISR(TIMER0_COMPA_vect)
{
    static uint8_t sample;
    // PORTB |= _BV(PB3);
    g_finished = snd_get_sample(&sample);
    OCR1A = sample;
    OCR1B = sample ^ 255;
    // PORTB &= ~_BV(PB3);
}

ISR(WDT_vect)
{
}

#ifdef USE_BH1750
static bool is_dark(void)
{
    uint16_t level;

    i2c_start(BH1750_ADDR, 0);
    i2c_write(ONE_TIME_HIGH_RES_MODE_2);
    i2c_stop();

    _delay_ms(180);

    i2c_start(BH1750_ADDR, 2);
    level = i2c_read() << 8;
    level |= i2c_read();
    i2c_stop();

    if (level < 3)
    {
        return true;
    }
    else
    {
        return false;
    }
}
#else
static bool is_dark(void)
{
    uint32_t adc_val = 0;

    PORTB |= _BV(PB3);
    for (uint8_t i = 0; i < 20; i++)
    {
        uint16_t adc_l = ADCL;
        adc_val += (ADCH << 8) | adc_l;
        ADCSRA |= _BV(ADIF);
        _delay_us(200);
    }

    adc_val /= 20;
    PORTB &= ~_BV(PB3);

    if (adc_val <= 80)
    {
        return true;
    }
    else
    {
        return false;
    }
}
#endif

void annoy_deep_sleep(uint8_t i)
{
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
        power_all_disable();
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    }

    while (i--)
    {
        MCUSR &= ~_BV(WDRF);
        WDTCR |= (_BV(WDCE) | _BV(WDE));
        WDTCR = _BV(WDP3) | _BV(WDP0);
        WDTCR |= _BV(WDIE);
#ifndef USE_BH1750
        ADCSRA &= ~_BV(ADEN);
#endif
        sleep_mode();
    }

    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
        power_all_enable();
        set_sleep_mode(SLEEP_MODE_IDLE);
#ifndef USE_BH1750
        ADCSRA |= _BV(ADEN);
#endif
    }
}

int main(void)
{
    bool finished;
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
        MCUSR &= ~_BV(WDRF);
        setup();
    }

    for (;;)
    {
        bool volatile is_drk = is_dark();
        if (is_drk)
        {
            TIMSK |= _BV(OCIE0A);
            do
            {
                sleep_mode();
                ATOMIC_BLOCK(ATOMIC_FORCEON)
                {
                    finished = g_finished;
                }
            } while (!finished);
            TIMSK &= ~_BV(OCIE0A);
            snd_wait();
        }
        else
        {
            annoy_deep_sleep(20);
        }
    }
}
