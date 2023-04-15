#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <util/delay.h>
#include <util/atomic.h>

#include <stdlib.h>
#include <stdbool.h>

#define NUM_LEDS (6)

static volatile uint8_t Levels[NUM_LEDS] = {63, 10, 0, 0, 0, 10};
static const uint8_t Order[NUM_LEDS] = {0, 2, 5, 1, 3, 4};

static void setup(void)
{
  // Set up Timer/Counter1 to multiplex the LEDs
  // Assumes 8MHz clock
  TCCR1 = _BV(CTC1) | 2 << CS10; // Divide by 2
  GTCCR = 0;                     // No PWM
  OCR1A = 0;
  OCR1C = 250 - 1;             // 16kHz
  TIMSK = TIMSK | 1 << OCIE1A; // Compare Match A interrupt
}

ISR(WDT_vect)
{
}

static bool g_done;

// Timer/Counter1 overflow interrupt
ISR(TIM1_COMPA_vect)
{
  static uint8_t first, ramp, column, bits, colbit;
  ramp = (ramp + 1) & 0x3F; // Count from 0 to 63
  if (ramp == 0)
  {
    bits = 0x03; // All on
    column = (column + 1) % 3;
    first = column * 2; // First LED in this column
    colbit = 1 << column;
    g_done = true;
  }

  if (Levels[Order[first]] == ramp)
    bits = bits & 0x02;
  if (Levels[Order[first + 1]] == ramp)
    bits = bits & 0x01;

  uint8_t mask = colbit - 1;
  uint8_t outputs = (bits & mask) | ((bits & ~mask) << 1);
  DDRB = (DDRB & 0xF8) | outputs | colbit;
  PORTB = (PORTB & 0xF8) | outputs;
}

static void clear(void)
{
  bool done;

  ATOMIC_BLOCK(ATOMIC_FORCEON)
  {
    for (uint8_t j = 0; j < NUM_LEDS; j++)
    {
      Levels[j] = 0;
    }
    g_done = false;
  }

  do
  {
    sleep_mode();
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
      done = g_done;
    }
  } while (!done);
}

static void spin(void)
{
  ATOMIC_BLOCK(ATOMIC_FORCEON)
  {
    uint8_t temp = Levels[NUM_LEDS - 1];
    for (int i = NUM_LEDS - 1; i > 0; i--)
      Levels[i] = Levels[i - 1];
    Levels[0] = temp;
  }
  _delay_ms(50);
}

static void deep_sleep(void)
{
  ATOMIC_BLOCK(ATOMIC_FORCEON)
  {
    power_all_disable();
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  }

  uint8_t i = 6 + (rand() % 10);

  while (i--)
  {
    MCUSR &= ~_BV(WDRF);
    WDTCR |= (_BV(WDCE) | _BV(WDE));
    WDTCR = _BV(WDP3) | _BV(WDP0);
    WDTCR |= _BV(WDIE);
    sleep_mode();
  }

  ATOMIC_BLOCK(ATOMIC_FORCEON)
  {
    power_all_enable();
    set_sleep_mode(SLEEP_MODE_IDLE);
  }
}

static void flicker(void)
{
  uint8_t rep = rand() % _BV(NUM_LEDS);
  rep |= rand() % _BV(NUM_LEDS);

  ATOMIC_BLOCK(ATOMIC_FORCEON)
  {
    for (uint8_t i = 0; i < NUM_LEDS; i++)
    {
      if (rep & _BV(i))
      {
        uint8_t l = rand() % 64;
        Levels[i] = l;
      }
    }
  }
  for (;;)
  {
    bool end = true;

    _delay_ms(20);
    for (uint8_t i = 0; i < NUM_LEDS; i++)
    {
      if (Levels[i] > 0)
      {
        Levels[i]--;
        end = false;
      }
    }

    if (end)
    {
      break;
    }
  }
}

int main(void)
{
  ATOMIC_BLOCK(ATOMIC_FORCEON)
  {
    MCUSR &= ~(1 << WDRF);
    ADCSRA &= ~(1 << ADEN);
    setup();
    set_sleep_mode(SLEEP_MODE_IDLE);
  }
  sei();

  for (size_t i = 0; i < 30; i++)
  {
    spin();
  }

  for (;;)
  {
    flicker();
    clear();
    deep_sleep();
  }

  return 0;
}
