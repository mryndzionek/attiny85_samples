#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <util/delay.h>

#define DS18B20_CONVERT_T (0x44)
#define DS18B20_READ (0xBE)

#define OW_PIN (PB4)
#define ONEWIRE_PIN_INPUT() (DDRB &= ~_BV(OW_PIN))
#define ONEWIRE_PIN_OUTPUT() (DDRB |= _BV(OW_PIN))
#define ONEWIRE_PIN_LOW() (PORTB &= ~_BV(OW_PIN))
#define ONEWIRE_PIN_HIGH() (PORTB |= _BV(OW_PIN))
#define ONEWIRE_PIN_READ() (PINB & _BV(OW_PIN))

#define ONEWIRE_RESET_RETRIES_MAX (128)
#define ONEWIRE_SKIP_ROM (0xCC)

ISR(WDT_vect) {}

static void setup_wdt() {
  cli();
  WDTCR |= (_BV(WDCE) | _BV(WDE));
  WDTCR = _BV(WDIE) | _BV(WDP3);  // ~4s
  sei();
}

static uint8_t onewire_reset(void) {
  uint8_t retval, retries;

  ONEWIRE_PIN_LOW();
  ONEWIRE_PIN_INPUT();

  retries = ONEWIRE_RESET_RETRIES_MAX;
  while (!ONEWIRE_PIN_READ()) {
    if (--retries == 0) {
      return (2);
    }
    _delay_us(1);
  }

  ONEWIRE_PIN_OUTPUT();
  _delay_us(480);
  ONEWIRE_PIN_INPUT();
  _delay_us(66);
  retval = ONEWIRE_PIN_READ();
  _delay_us(414);

  return (retval);
}

static uint8_t onewire_bit(uint8_t value) {
  cli();
  ONEWIRE_PIN_OUTPUT();
  _delay_us(1);
  if (value) {
    ONEWIRE_PIN_INPUT();
  }
  _delay_us(14);
  value = !(ONEWIRE_PIN_READ() == 0);
  _delay_us(45);
  ONEWIRE_PIN_INPUT();
  sei();

  return value;
}

static uint8_t onewire_write(uint8_t value) {
  uint8_t i, r;

  for (i = 0; i < 8; ++i) {
    r = onewire_bit(value & 0x01);
    value >>= 1;
    if (r) {
      value |= 0x80;
    }
  }

  return value;
}

uint8_t onewire_read(void) { return onewire_write(0xff); }

static int16_t read_ext_temp(void) {
  uint8_t msb, lsb;
  int8_t sign = 1;
  uint16_t t;

  onewire_reset();
  onewire_write(ONEWIRE_SKIP_ROM);
  onewire_write(DS18B20_CONVERT_T);

  onewire_reset();
  onewire_write(ONEWIRE_SKIP_ROM);
  onewire_write(DS18B20_READ);

  lsb = onewire_read();
  msb = onewire_read();
  t = ((uint16_t)msb << 8) | lsb;

  if ((msb & 0xf8) == 0xf8) {
    t = (65536 - t);
    sign = -1;
  }

  return sign * (((uint16_t)t * 10U) / 16U);
}

int main(void) {
  ADCSRA &= ~(1 << ADEN);  // ADC off
  power_all_disable();

  DDRB |= 1 << PB3;

  setup_wdt();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  uint8_t count = 0;

  for (;;) {
    sleep_mode();
    count++;
    if (count == 1) {
      PORTB |= _BV(PB3);
      const int16_t temp = read_ext_temp();
      uint32_t data = ((uint32_t)((uint16_t)temp)) << 8;
      uint8_t crc = ((data >> 8) & 0xFF) ^ ((data >> 16) & 0xFF);
      data |= crc;
      count = 0;
      PORTB &= ~_BV(PB3);
    }
  }

  return 0;
}
