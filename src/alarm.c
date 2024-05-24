#include <stdbool.h>
#include <stdlib.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>

#include <util/delay.h>
#include <util/atomic.h>

#define PWM_PIN (PB4)
#define BUTTON_PIN (PB2)

#define START_FREQ (167) // 1.5kHz
#define STOP_FREQ (100)  // 2.5kHz

static bool volatile sound_on = true;
static uint8_t volatile freq = START_FREQ;

ISR(TIM1_COMPA_vect)
{
    if (sound_on)
    {
        OCR1B = 50 + rand() % 128;
    }
    else
    {
        OCR1B = 0;
    }
}

static void setup_pwm(void)
{
    TIMSK = 0;
    // Set prescaler to 8 (8MHz / 32 = 250kHz)
    TCCR1 |= _BV(CS12) | _BV(CS11);

    // compare value and top value
    OCR1B = 128; // raise to increase PWM duty
    OCR1C = freq - 1;

    // Enable OCRB output on PB4
    DDRB |= _BV(PWM_PIN);

    // toggle PB4 when when timer reaches OCR1B (target)
    GTCCR |= _BV(COM1B0);

    // clear PB4 when when timer reaches OCR1C (top)
    GTCCR |= _BV(PWM1B);

    // sometimes required for old, glitchy chips.
    TCCR1 |= _BV(COM1A0);
    TIMSK = _BV(OCIE1A);
}

static bool button_deb(void)
{
    static uint8_t deb_counter = 0;
    static bool state = false;
    bool ret = false;

    switch (state)
    {
    case false:
        if (!((PINB >> BUTTON_PIN) & 0x01))
        {
            deb_counter++;
            if (deb_counter == 30)
            {
                ret = true;
                state = true;
            }
        }
        else
        {
            if (deb_counter > 0)
            {
                deb_counter--;
            }
        }
        break;

    case true:
        if (((PINB >> BUTTON_PIN) & 0x01))
        {
            deb_counter--;
            if (deb_counter == 0)
            {
                state = false;
            }
        }
        else
        {
            if (deb_counter < 30)
            {
                deb_counter++;
            }
        }
        break;
    }

    return ret;
}

int main(void)
{
    MCUSR &= ~_BV(WDRF);
    ADCSRA &= ~_BV(ADEN);
    PORTB |= _BV(BUTTON_PIN);

    set_sleep_mode(SLEEP_MODE_IDLE);

    uint8_t button_count = 0;
    uint16_t button_timeout = 0;

    while (true)
    {
        if (button_deb())
        {
            button_count++;
            if (button_count == 1)
            {
                button_timeout = 500;
            }
        }
        _delay_ms(1);

        if (button_timeout)
        {
            if (button_count == 3)
            {
                break;
            }
            button_timeout--;
        }
        else
        {
            button_count = 0;
        }
    }

    setup_pwm();
    sei();

    while (true)
    {
        _delay_ms(1);
        if (sound_on)
        {
            OCR1C = freq - 1;
        }

        freq-=2;
        if (freq <= STOP_FREQ)
        {
            freq = START_FREQ;
            sound_on ^= 1;
        }
    }
}
