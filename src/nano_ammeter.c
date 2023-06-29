#include <stdlib.h>

#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

#include <u8g2.h>
#include <u8x8_avr.h>

#define SSD1306_ADDR (0x78)
#define NUM_DIGITS (6)

#define MEASUREMENT_PIN (PB4)

static u8g2_t u8g2;

static const char *num_to_str(uint32_t num)
{
  static char buf[NUM_DIGITS + 1] = {0};
  uint32_t k = num;

  for (uint8_t j = 0; j < NUM_DIGITS; j++)
  {
    buf[NUM_DIGITS - 1 - j] = '0' + (k % 10);
    k /= 10;
  }

  return buf;
}

static void setup_main_timer(void)
{
  TCCR0A = 0x00; // Normal mode
  TCCR0B = 0x00;
  TCCR0B |= _BV(CS01) | _BV(CS00); // prescaling clk / 64
  TCCR0A |= _BV(WGM01);
  OCR0A = 125 - 1; // 125kHz / 125 = 1kHz = 1ms
  TCNT0 = 0;
  TIMSK |= _BV(OCIE0A);
}

static void adc_init(void)
{
  ADMUX = _BV(1); // configuring PB4 to take input
  ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
  ADCSRA |= _BV(ADEN);
}

static uint16_t adc_read(void)
{
  uint16_t adc_val = 0;

  ADCSRA |= _BV(ADSC);
  while (ADCSRA & _BV(ADSC))
  {
    _delay_us(10);
  }
  uint16_t adc_l = ADCL;
  adc_val = (ADCH << 8) | adc_l;
  ADCSRA |= _BV(ADIF);

  return adc_val;
}

// U0 = 5
// x = 4
// C = 1e-6

// mult = round((1e3 * 1e9) * U0 * math.log(2) * C / x)
// ratio = pow(math.e, -math.log(2) / x)
// ratio = round(ratio, 3)
// frac = Fraction(str(ratio))

static uint8_t measure(uint32_t *nA)
{
  uint32_t elapsed_ms = 0;

  // charge capacitor
  PORTB |= _BV(MEASUREMENT_PIN);
  _delay_ms(500);

  // start measurement loop
  DDRB &= ~_BV(MEASUREMENT_PIN);
  PORTB &= ~_BV(MEASUREMENT_PIN);

  // ratio: e^(ln(2) / x), x = 4
  uint16_t adc_target = (((uint32_t)adc_read()) * 841) / 1000;
  _delay_us(200);

  PORTB |= _BV(PB3);
  TCNT0 = 0;
  sei();

  do
  {
    sleep_mode();
    elapsed_ms++;
  } while ((adc_read() > adc_target) && (elapsed_ms < 50000));

  if (elapsed_ms > 50000)
  {
    return 1;
  }

  DDRB |= _BV(MEASUREMENT_PIN);
  PORTB |= _BV(MEASUREMENT_PIN);
  cli();
  PORTB ^= _BV(PB3);

  // multiplier: (1e3 * 1e9) * U0 * ln(2) * C / x, x = 4
  *nA = 866434 / elapsed_ms;
  if (*nA > 100000)
  {
    return 2;
  }
  else
  {

    return 0;
  }
}

ISR(TIMER0_COMPA_vect)
{
}

int main(void)
{
  DDRB |= _BV(MEASUREMENT_PIN) | _BV(PB3);
  PORTB &= ~(_BV(MEASUREMENT_PIN) | _BV(PB3));
  adc_init();
  setup_main_timer();

  u8g2_Setup_ssd1306_i2c_128x64_noname_1(&u8g2, U8G2_R0, u8x8_byte_avr_hw_i2c, u8x8_avr_delay);
  u8g2_SetI2CAddress(&u8g2, SSD1306_ADDR >> 1);
  u8g2_InitDisplay(&u8g2);
  u8g2_SetPowerSave(&u8g2, 0);
  u8g2_SetContrast(&u8g2, 0xff);
  u8g2_SetFlipMode(&u8g2, 1);
  u8g2_ClearDisplay(&u8g2);

  const char *str;

  while (1)
  {
    uint32_t na;
    uint8_t res = measure(&na);
    if (res == 0)
    {
      str = num_to_str(na);
    }

    u8g2_FirstPage(&u8g2);
    do
    {
      u8g2_SetFont(&u8g2, u8g2_font_tenthinguys_tr);
      u8g2_DrawStr(&u8g2, 100, 15, "nA");
      if (res == 1)
      {
        u8g2_DrawStr(&u8g2, 10, 15, "too low!");
      }
      else if (res == 2)
      {
        u8g2_DrawStr(&u8g2, 10, 15, "too high!");
      }
      u8g2_SetFont(&u8g2, u8g2_font_7_Seg_33x19_mn);
      if (res == 0)
      {
        u8g2_DrawStr(&u8g2, 0, 18, str);
      }
      else
      {
        u8g2_DrawStr(&u8g2, 0, 18, "------");
      }
    } while (u8g2_NextPage(&u8g2));
  }

  return 0;
}
