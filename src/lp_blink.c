#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <util/delay.h>

ISR(WDT_vect)
{
}

static void setup_wdt()
{
  cli();
  WDTCR |= (_BV(WDCE) | _BV(WDE));
  WDTCR = _BV(WDIE) |
          _BV(WDP1) | _BV(WDP0); // Set Timeout to ~0.125 seconds (or something)
  sei();
}

int main(void)
{
  ADCSRA &= ~(1 << ADEN); // ADC off
  power_all_disable();

  DDRB |= 1 << PB3;

  setup_wdt();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  for (;;)
  {
    sleep_mode();
    PORTB |= _BV(PB3);
    sleep_mode();
    PORTB &= ~_BV(PB3);
  }

  return 0;
}
