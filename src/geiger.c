#include <stdbool.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>

#include <util/delay.h>
#include <util/atomic.h>

#define PWM_DUTY (180)

#define LED_GREEN_PIN (PB2)
#define LED_RED_PIN (PB0)

#define ORANGE_THRESHOLD (50) // 0.5 uS/h
#define RED_THRESHOLD (1000)  // 10 uS/h

#define BOOST_INTERVAL_TIMEOUT_US (4000)
#define SHUTDOWN_TIMEOUT_US (100)
#define PULSE_STRETCHER_TIMEOUT_US (100)
#define DOSE_CALC_TIMEOUT_MS (5000)
#define CPM_BUF_SIZE (60000UL / DOSE_CALC_TIMEOUT_MS)

#define EV_BOOST_TIMEOUT _BV(0)
#define EV_SHUTDOWN_TIMEOUT _BV(1)
#define EV_POWER_DOWN_TIMEOUT _BV(2)
#define EV_PULSE_STRETCH_TIMEOUT _BV(3)
#define EV_GEIGER_PULSE _BV(4)
#define EV_DOSE_CALC_TIMEOUT _BV(5)

#define S1_ID _BV(0)
#define S2_ID _BV(1)

// _tm can be t1, t2, or t4
// resolution is 32us
#define TIMER_TRIGGER_US(_tm, _us) \
  do                               \
  {                                \
    ATOMIC_BLOCK(ATOMIC_FORCEON)   \
    {                              \
      _tm = 1 + (_us >> 5);        \
    }                              \
  } while (0)

// _tm can be t5
// resolution is 32ms
#define TIMER_LP_TRIGGER_MS(_tm, _ms) \
  do                                  \
  {                                   \
    ATOMIC_BLOCK(ATOMIC_FORCEON)      \
    {                                 \
      _tm = 1 + (_ms >> 5);           \
    }                                 \
  } while (0)

#define IS_EVENT(_ev) (_events_ & (_ev))

#define CLEAR_EVENT(_ev) \
  do                     \
  {                      \
    _events_ &= ~(_ev);  \
  } while (0)

typedef enum
{
  s1_boosting = 0,
  s1_powering_down,
  s1_power_down,
} s1_e;

typedef enum
{
  s2_idle = 0,
  s2_pulse,
} s2_e;

static volatile uint_fast8_t events;
static volatile uint_fast16_t t1;
static volatile uint_fast16_t t2;
static volatile uint_fast16_t t4;
static volatile uint_fast16_t t5;
static volatile uint_fast16_t pulse_count;

static void setup_pwm(void)
{
  // Enable PLL (64 MHz)
  PLLCSR |= _BV(PLLE);

  // Use PLL as timer clock source
  PLLCSR |= _BV(PCKE);

  // Set prescaler to 32 (64MHz / 32 = 2MHz)
  TCCR1 |= _BV(CS11) | _BV(CS12);

  // compare value and top value
  OCR1B = 0;       // raise to increase PWM duty
  OCR1C = 250 - 1; // 2MHz / 250 = ~8kH

  // Enable OCRB output on PB4
  DDRB |= _BV(PB4);

  // toggle PB4 when when timer reaches OCR1B (target)
  GTCCR |= _BV(COM1B0);

  // clear PB4 when when timer reaches OCR1C (top)
  GTCCR |= _BV(PWM1B);

  // sometimes required for old, glitchy chips.
  TCCR1 |= _BV(COM1A0);
}

static void setup_comparator(void)
{
  ACSR &= ~_BV(ACD);
  ADCSRA = 0x00; // Using AIN0 and AIN1 as comparator inputs
  ADCSRB = 0x00;
  DIDR0 |= _BV(AIN1D) | _BV(AIN0D);
  ACSR |= _BV(ACBG);
  // ACSR |= _BV(ACIE);
}

static void setup_wdt(void)
{
  MCUSR &= ~_BV(WDRF);
  WDTCR |= (_BV(WDCE) | _BV(WDE));
  WDTCR = _BV(WDP0); // Set Timeout to 32ms
  WDTCR = _BV(WDIE);
}

static void setup_main_timer(void)
{
  TCCR0A = 0x00; // Normal mode
  TCCR0B = 0x00;
  TCCR0B |= _BV(CS00); // no prescaling
  TCNT0 = 0;
  TIMSK |= _BV(TOIE0);
}

static bool v_too_low(void)
{
  uint_fast8_t count = 0;
  uint_fast8_t i = 10;

  do
  {
    if (ACSR & _BV(ACO))
    {
      count++;
    }
    i--;
  } while (i);

  return (count > 5);
}

ISR(WDT_vect)
{
  events |= EV_POWER_DOWN_TIMEOUT;

  if (t5 > 0)
  {
    t5--;
    if (t5 == 0)
    {
      events |= EV_DOSE_CALC_TIMEOUT;
      t5 = 1 + (DOSE_CALC_TIMEOUT_MS >> 5);
    }
  }
}

ISR(TIMER0_OVF_vect)
{
  if (t1 > 0)
  {
    t1--;
    if (t1 == 0)
    {
      events |= EV_BOOST_TIMEOUT;
    }
  }

  if (t2 > 0)
  {
    t2--;
    if (t2 == 0)
    {
      events |= EV_SHUTDOWN_TIMEOUT;
    }
  }

  if (t4 > 0)
  {
    t4--;
    if (t4 == 0)
    {
      events |= EV_PULSE_STRETCH_TIMEOUT;
    }
  }
}

ISR(PCINT0_vect)
{
  // seems to be okay
  // but debouncing can be added
  if (!(PINB & _BV(PB3)))
  {
    pulse_count++;
    events |= EV_GEIGER_PULSE;
  }
}

int main(void)
{
  s1_e s1 = s1_power_down;
  s2_e s2 = s2_idle;
  uint_fast8_t power_flags = 0;
  uint_fast8_t _events_ = 0;
  uint_fast16_t cpm_buf[CPM_BUF_SIZE] = {0};
  uint_fast8_t cpm_i = 0;
  uint_fast16_t cpm = 0;
  uint_fast16_t dose = 0;

  MCUSR &= ~_BV(WDRF);
  ADCSRA &= ~_BV(ADEN);

  DDRB |= _BV(PB4) | _BV(LED_RED_PIN) | _BV(LED_GREEN_PIN);
  PORTB &= ~(_BV(LED_RED_PIN) | _BV(LED_GREEN_PIN));

  setup_pwm();
  setup_main_timer();
  setup_comparator();
  setup_wdt();

  power_all_disable();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  // Enable PCINT3 (PB3)
  GIMSK |= _BV(PCIE);
  PCMSK |= _BV(PCINT3);

  TIMER_LP_TRIGGER_MS(t5, DOSE_CALC_TIMEOUT_MS);

  for (;;)
  {
    cli();
    _events_ |= events;
    events = 0;
    if (events == 0)
    {
      if (power_flags == 0)
      {
        TCNT0 = 0;
        power_all_disable();
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        setup_wdt();
      }
      else
      {
        set_sleep_mode(SLEEP_MODE_IDLE);
      }

      do
      {
        sleep_enable();
        sei();
        sleep_cpu();
        sleep_disable();

        cli();
        _events_ |= events;
        events = 0;
      } while (_events_ == 0);

      sei();

      if (power_flags == 0)
      {
        power_timer0_enable();
        power_timer1_enable();
        PLLCSR |= _BV(PLLE);
        PLLCSR |= _BV(PCKE);
      }
    }
    sei();

    switch (s1)
    {
    case s1_power_down:
      if IS_EVENT (EV_POWER_DOWN_TIMEOUT)
      {
        if (v_too_low())
        {
          OCR1B = PWM_DUTY;
          s1 = s1_boosting;
          TIMER_TRIGGER_US(t1, BOOST_INTERVAL_TIMEOUT_US);
          power_flags |= S1_ID;
        }
        CLEAR_EVENT(EV_POWER_DOWN_TIMEOUT);
      }
      break;

    case s1_boosting:
      if IS_EVENT (EV_BOOST_TIMEOUT)
      {
        if (!v_too_low())
        {
          OCR1B = 0;
          s1 = s1_powering_down;
          TIMER_TRIGGER_US(t2, SHUTDOWN_TIMEOUT_US);
        }
        else
        {
          TIMER_TRIGGER_US(t1, BOOST_INTERVAL_TIMEOUT_US);
        }
        CLEAR_EVENT(EV_BOOST_TIMEOUT);
      }
      break;

    case s1_powering_down:
      if IS_EVENT (EV_SHUTDOWN_TIMEOUT)
      {
        s1 = s1_power_down;
        power_flags &= ~S1_ID;
        CLEAR_EVENT(EV_SHUTDOWN_TIMEOUT);
      }
      break;
    }

    switch (s2)
    {
    case s2_idle:
      if IS_EVENT (EV_GEIGER_PULSE)
      {
        if (dose < ORANGE_THRESHOLD)
        {
          PORTB |= _BV(LED_GREEN_PIN);
        }
        else if (dose < RED_THRESHOLD)
        {
          PORTB |= _BV(LED_GREEN_PIN) | _BV(LED_RED_PIN);
        }
        else
        {
          PORTB |= _BV(LED_RED_PIN);
        }
        s2 = s2_pulse;
        power_flags |= S2_ID;
        TIMER_TRIGGER_US(t4, PULSE_STRETCHER_TIMEOUT_US);
        CLEAR_EVENT(EV_GEIGER_PULSE);
      }
      break;

    case s2_pulse:
      if IS_EVENT (EV_PULSE_STRETCH_TIMEOUT)
      {
        PORTB &= ~(_BV(LED_RED_PIN) | _BV(LED_GREEN_PIN));
        s2 = s2_idle;
        power_flags &= ~S2_ID;
        CLEAR_EVENT(EV_PULSE_STRETCH_TIMEOUT);
      }
      break;
    }

    if IS_EVENT (EV_DOSE_CALC_TIMEOUT)
    {
      uint_fast16_t pc;
      ATOMIC_BLOCK(ATOMIC_FORCEON)
      {
        pc = pulse_count;
        pulse_count = 0;
      }
      // dead time correction
      uint_fast16_t pc5s = (pc >> 5);
      pc5s *= pc5s;
      pc5s /= 20;
      pc5s += pc;

      // 1-minute rolling sum
      cpm -= cpm_buf[cpm_i];
      cpm += pc5s;
      cpm_buf[cpm_i] = pc5s;
      ++cpm_i;
      if (cpm_i > (CPM_BUF_SIZE - 1))
      {
        cpm_i = 0;
      }

      // dose_rate in 0.01 µSv/hr
      dose = cpm >> 1;
      dose += dose / 25;
      CLEAR_EVENT(EV_DOSE_CALC_TIMEOUT);
    }
  }

  return 0;
}
