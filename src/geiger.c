#include <stdbool.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>

#include <util/delay.h>
#include <util/atomic.h>

// Remember to set F_CPU to 8Mhz - '-U lfuse:w:0xE2:m'

// Uncommenting this will enable
// the Attiny85's RESET (PB5) line control (used for buzzer clicks).
// Fuses need to be changed for that to (-U hfuse:w:0x5F:m)
// #define ENABLE_RESET_PIN

// In this mode clicks are enabled regardless of the dose
// and red LED flashes when the boost converter is enabled
// #define HV_TEST_MODE

#define PWM_DUTY (230)

#define LED_GREEN_PIN (PB0)
#define LED_RED_PIN (PB2)
#define BOOST_PWM_PIN (PB4)
#ifdef ENABLE_RESET_PIN
#define BUZZER_PIN (PB5)
#endif

#define ORANGE_THRESHOLD (50) // 0.5 µS/h
#define RED_THRESHOLD (1000)  // 10 µS/h

#define BOOST_INTERVAL_TIMEOUT_US (2000)
#define SHUTDOWN_TIMEOUT_US (400)
#define LED_PULSE_TIMEOUT_US (20000)
#ifdef ENABLE_RESET_PIN
#define BUZZER_PULSE_TIMEOUT_US (200)
#define BUZZER_FALLOFF_TIMEOUT_US (2000)
#endif

#define DOSE_CALC_TIMEOUT_MS (5000)
#define CPM_BUF_SIZE (60000UL / DOSE_CALC_TIMEOUT_MS)

#define EV_BOOST_TIMEOUT _BV(0)
#define EV_SHUTDOWN_TIMEOUT _BV(1)
#define EV_POWER_DOWN_TIMEOUT _BV(2)
#define EV_LED_PULSE_TIMEOUT _BV(3)
#define EV_GEIGER_PULSE _BV(4)
#define EV_GEIGER_PULSE_2 _BV(5)
#define EV_DOSE_CALC_TIMEOUT _BV(6)
#ifdef ENABLE_RESET_PIN
#define EV_BUZZER_PULSE_TIMEOUT _BV(7)
#endif

#define S1_ID _BV(0)
#define S2_ID _BV(1)
#ifdef ENABLE_RESET_PIN
#define S3_ID _BV(2)
#endif

// _tm can be t1, t2 or t4 (or t6)
// resolution is 100us
#define TIMER_TRIGGER_US(_tm, _us) \
  do                               \
  {                                \
    ATOMIC_BLOCK(ATOMIC_FORCEON)   \
    {                              \
      _tm = 1 + (_us / 100);       \
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

#ifdef ENABLE_RESET_PIN
typedef enum
{
  s3_idle = 0,
  s3_pulse,
  s3_falloff,
} s3_e;
#endif

static volatile uint_fast8_t events;
static volatile uint_fast8_t t1;
static volatile uint_fast8_t t2;
static volatile uint_fast8_t t4;
static volatile uint_fast8_t t5;
#ifdef ENABLE_RESET_PIN
static volatile uint_fast8_t t6;
#endif
static volatile uint_fast16_t pulse_count;

static void setup_pwm(void)
{
  // Set prescaler to 8 (8MHz / 8 = 1MHz)
  TCCR1 |= _BV(CS12);

  // compare value and top value
  OCR1B = 0;       // raise to increase PWM duty
  OCR1C = 250 - 1; // 1MHz / 250 = 4kH

  // Enable OCRB output on PB4
  DDRB |= _BV(BOOST_PWM_PIN);

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
}

static void setup_wdt(void)
{
  MCUSR &= ~_BV(WDRF);
  WDTCR |= (_BV(WDCE) | _BV(WDE));
  WDTCR = _BV(WDIE) | _BV(WDP0); // Set Timeout to 64ms
}

static void setup_main_timer(void)
{
  TCCR0A = 0x00; // Normal mode
  TCCR0B = 0x00;
  TCCR0B |= _BV(CS01); // prescaling clk / 8
  TCCR0A |= _BV(WGM01);
  OCR0A = 100 - 1; // 1MHz / 100 = 10kHz = 100us
  TCNT0 = 0;
  TIMSK |= _BV(OCIE0A);
}

static bool v_too_low(void)
{
  uint_fast8_t count = 0;
  uint_fast8_t i = 20;

  do
  {
    if (ACSR & _BV(ACO))
    {
      count++;
    }
  } while (--i);

  return (count > 5);
}

ISR(WDT_vect)
{
  events |= EV_POWER_DOWN_TIMEOUT;

  if (t5 > 0)
  {
    if (--t5 == 0)
    {
      events |= EV_DOSE_CALC_TIMEOUT;
      TIMER_LP_TRIGGER_MS(t5, DOSE_CALC_TIMEOUT_MS);
    }
  }
}

ISR(TIMER0_COMPA_vect)
{
  if (t1 > 0)
  {
    if (--t1 == 0)
    {
      events |= EV_BOOST_TIMEOUT;
    }
  }

  if (t2 > 0)
  {
    if (--t2 == 0)
    {
      events |= EV_SHUTDOWN_TIMEOUT;
    }
  }

  if (t4 > 0)
  {
    if (--t4 == 0)
    {
      events |= EV_LED_PULSE_TIMEOUT;
    }
  }

#ifdef ENABLE_RESET_PIN
  if (t6 > 0)
  {
    if (--t6 == 0)
    {
      events |= EV_BUZZER_PULSE_TIMEOUT;
    }
  }
#endif
}

ISR(PCINT0_vect)
{
  // seems to be okay
  // but debouncing can be added
  if (!(PINB & _BV(PB3)))
  {
    pulse_count++;
    events |= EV_GEIGER_PULSE;
#ifdef ENABLE_RESET_PIN
    events |= EV_GEIGER_PULSE_2;
#endif
  }
}

int main(void)
{
  s1_e s1 = s1_power_down;
  s2_e s2 = s2_idle;
#ifdef ENABLE_RESET_PIN
  s3_e s3 = s3_idle;
#endif

  uint_fast8_t power_flags = 0;
  uint_fast8_t _events_ = 0;
  uint_fast16_t cpm_buf[CPM_BUF_SIZE] = {0};
  uint_fast8_t cpm_i = 0;
  uint_fast16_t cpm = 0;
  uint_fast16_t dose = 0;

  MCUSR &= ~_BV(WDRF);
  ADCSRA &= ~_BV(ADEN);

  DDRB |= _BV(BOOST_PWM_PIN) | _BV(LED_RED_PIN) | _BV(LED_GREEN_PIN);
#ifdef ENABLE_RESET_PIN
  DDRB |= _BV(BUZZER_PIN);
#endif
  PORTB |= _BV(LED_RED_PIN) | _BV(LED_GREEN_PIN);
#ifdef ENABLE_RESET_PIN
  PORTB &= ~_BV(BUZZER_PIN);
#endif

  // just a reset indication
  PORTB &= ~_BV(LED_RED_PIN);
  _delay_ms(1000);
  PORTB |= _BV(LED_RED_PIN);

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
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
      _events_ |= events;
      events = 0;
      if (_events_ == 0)
      {
        if (power_flags == 0)
        {
          TCNT0 = 0;
          power_timer0_disable();
          power_timer1_disable();
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
          NONATOMIC_BLOCK(NONATOMIC_FORCEOFF)
          {
            sleep_cpu();
            sleep_disable();
          }
          _events_ |= events;
          events = 0;
        } while (_events_ == 0);

        if (power_flags == 0)
        {
          power_timer0_enable();
          power_timer1_enable();
        }
      }
    }

    switch (s1)
    {
    case s1_power_down:
      if IS_EVENT (EV_POWER_DOWN_TIMEOUT)
      {
        if (v_too_low())
        {
#ifdef HV_TEST_MODE
          PORTB &= ~_BV(LED_RED_PIN);
#endif
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
#ifdef HV_TEST_MODE
          PORTB |= _BV(LED_RED_PIN);
#endif
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
#ifndef HV_TEST_MODE
        if (dose < ORANGE_THRESHOLD)
        {
          PORTB &= ~_BV(LED_GREEN_PIN);
        }
        else if (dose < RED_THRESHOLD)
        {
          PORTB &= ~(_BV(LED_GREEN_PIN) | _BV(LED_RED_PIN));
        }
        else
        {
          PORTB &= ~_BV(LED_RED_PIN);
        }
#endif
        s2 = s2_pulse;
        power_flags |= S2_ID;
        TIMER_TRIGGER_US(t4, LED_PULSE_TIMEOUT_US);
      }
      break;

    case s2_pulse:
      if IS_EVENT (EV_LED_PULSE_TIMEOUT)
      {
#ifndef HV_TEST_MODE
        PORTB |= _BV(LED_RED_PIN) | _BV(LED_GREEN_PIN);
#endif
        s2 = s2_idle;
        power_flags &= ~S2_ID;
        CLEAR_EVENT(EV_LED_PULSE_TIMEOUT);
      }
      break;
    }
    CLEAR_EVENT(EV_GEIGER_PULSE);

#ifdef ENABLE_RESET_PIN
    switch (s3)
    {
    case s3_idle:
      if IS_EVENT (EV_GEIGER_PULSE_2)
      {
#ifndef HV_TEST_MODE
        if (dose >= ORANGE_THRESHOLD)
        {
#endif
          PORTB |= _BV(BUZZER_PIN);
          s3 = s3_pulse;
          power_flags |= S3_ID;
          TIMER_TRIGGER_US(t6, BUZZER_PULSE_TIMEOUT_US);
#ifndef HV_TEST_MODE
        }
#endif
      }
      break;

    case s3_pulse:
      if IS_EVENT (EV_BUZZER_PULSE_TIMEOUT)
      {
        PORTB &= ~_BV(BUZZER_PIN);
        s3 = s3_falloff;
        TIMER_TRIGGER_US(t6, BUZZER_FALLOFF_TIMEOUT_US);
        CLEAR_EVENT(EV_BUZZER_PULSE_TIMEOUT);
      }
      break;

    case s3_falloff:
      if IS_EVENT (EV_BUZZER_PULSE_TIMEOUT)
      {
        s3 = s3_idle;
        power_flags &= ~S3_ID;
        CLEAR_EVENT(EV_BUZZER_PULSE_TIMEOUT);
      }
      break;
    }
    CLEAR_EVENT(EV_GEIGER_PULSE_2);
#endif

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
      if (cpm_i == CPM_BUF_SIZE)
      {
        cpm_i = 0;
      }

      // dose_rate in 0.01 µSv/hr

      // // this calculation is for SBM-20 tube,
      // // with conversion factor of k = 0.0052
      // dose = cpm >> 1;
      // dose += dose / 25;

      // this calculation is for J305 tube,
      // (some sources seem to indicate
      // that this is in fact also the correct
      // value for SBM-20 tube)
      // with conversion factor of k = 0.00812
      dose = cpm >> 1;
      dose += (dose >> 1) + (dose >> 3);

      CLEAR_EVENT(EV_DOSE_CALC_TIMEOUT);
    }
  }

  return 0;
}
