#include <assert.h>
#include <avr/eeprom.h>
#include <avr/io.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <math.h>
#include <stdbool.h>
#include <util/atomic.h>
#include <util/delay.h>

#include "light_ws2812.h"

#define LED_PIN (PB4)
#define IR_PIN (PB2)

#define NUM_LEDS (120)

#define IR_ADDR (0xFF00)

#define IR_CMD_MINUS (0xF807)
#define IR_CMD_PLUS (0xEA15)
#define IR_CMD_EQ (0xF609)
#define IR_CMD_CH (0xB946)
#define IR_CMD_CHMINUS (0xBA45)
#define IR_CMD_CHPLUS (0xB847)
#define IR_CMD_LEFT (0xBB44)
#define IR_CMD_RIGHT (0xBF40)

#define IR_CMD_ZERO (0xE916)
#define IR_CMD_ONE (0xF30C)
#define IR_CMD_TWO (0xE718)
#define IR_CMD_THREE (0xA15E)
#define IR_CMD_NINE (0xB54A)

#define TIMEOUT ((uint32_t)(3 * 60.0f / 0.125f))  // 3 minutes

#define EV_IR _BV(0)
#define EV_REPEAT _BV(1)
#define EV_TICK _BV(2)

typedef struct {
  uint8_t g;
  uint8_t r;
  uint8_t b;
} CRGB_t;

typedef struct {
  uint16_t H;
  float S;
  float V;
} hsv_t;

typedef struct {
  uint8_t events;
  uint32_t ir_data;
} state_t;

static CRGB_t leds[NUM_LEDS];
static_assert(sizeof(leds) == NUM_LEDS * sizeof(CRGB_t));

static volatile state_t g_state;
static hsv_t g_hsv = {0, 1.0, 1.0};
static uint16_t eeprom_H EEMEM = 0;
static float eeprom_S EEMEM = 1.0f;

static inline void ReceivedCode(bool Repeat) {
  g_state.events |= EV_IR;
  if (Repeat) {
    g_state.events |= EV_REPEAT;
  }
}

ISR(WDT_vect) { g_state.events |= EV_TICK; }

ISR(INT0_vect) {
  static uint8_t NextBit = 32;

  const uint8_t Time = TCNT0;
  const uint8_t Overflow = TIFR & _BV(TOV0);
  // AGC pulse and gap
  if (NextBit == 32) {
    if ((Time >= 194 / 2) && (Time <= 228 / 2) && (Overflow == 0)) {
      g_state.ir_data = 0;
      NextBit = 0;
    } else if ((Time >= 159 / 2) && (Time <= 193 / 2) && (Overflow == 0))
      ReceivedCode(1);
    // Data bit
  } else {
    if ((Time > 44 / 2) || (Overflow != 0))
      NextBit = 32;  // Invalid - restart
    else {
      if (Time > 26 / 2) g_state.ir_data |= ((unsigned long)1 << NextBit);
      if (NextBit == 31) ReceivedCode(0);
      NextBit++;
    }
  }
  TCNT0 = 0;
  TIFR = TIFR | _BV(TOV0);
  GIFR = GIFR | _BV(INTF0);
}

static CRGB_t hsv_to_rgb(hsv_t hsv) {
  float r = 0, g = 0, b = 0;

  if (hsv.S == 0) {
    r = hsv.V;
    g = hsv.V;
    b = hsv.V;
  } else {
    int i;
    float f, p, q, t, H;

    hsv.H %= 360;
    if (hsv.H == 360)
      H = 0.0;
    else
      H = hsv.H / 60.0;

    i = (int)trunc(H);
    f = H - i;

    p = hsv.V * (1.0 - hsv.S);
    q = hsv.V * (1.0 - (hsv.S * f));
    t = hsv.V * (1.0 - (hsv.S * (1.0 - f)));

    switch (i) {
      case 0:
        r = hsv.V;
        g = t;
        b = p;
        break;

      case 1:
        r = q;
        g = hsv.V;
        b = p;
        break;

      case 2:
        r = p;
        g = hsv.V;
        b = t;
        break;

      case 3:
        r = p;
        g = q;
        b = hsv.V;
        break;

      case 4:
        r = t;
        g = p;
        b = hsv.V;
        break;

      default:
        r = hsv.V;
        g = p;
        b = q;
        break;
    }
  }

  return (CRGB_t){
      .r = r * 255,
      .g = g * 255,
      .b = b * 255,
  };
}

static void update_leds(void) {
  CRGB_t rgb = hsv_to_rgb(g_hsv);
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i] = rgb;
  }
  ws2812_sendarray((uint8_t*)leds, sizeof(leds));
}

static inline void ack(uint8_t count) {
  for (uint8_t i = 0; i < count; i++) {
    _delay_ms(100);
    g_hsv.V = 0.0f;
    update_leds();
    _delay_ms(100);
    g_hsv.V = 1.0f;
    update_leds();
  }
}

static inline void setup_wdt(void) {
  MCUSR &= ~_BV(WDRF);
  WDTCR |= (_BV(WDCE) | _BV(WDE));
  WDTCR = _BV(WDIE) | _BV(WDP0) | _BV(WDP1);  // Set Timeout to 125ms
}

static void eeprom_read(void) {
  uint16_t def_H = eeprom_read_word(&eeprom_H);
  if (def_H > 360) {
    def_H = 0;
    eeprom_update_word(&eeprom_H, def_H);
  }
  g_hsv.H = def_H;
  float def_S = eeprom_read_float(&eeprom_S);
  if ((def_S > 1.0f) || (def_S < 0.0f)) {
    def_S = 1.0;
    eeprom_update_float(&eeprom_S, def_S);
  }
  g_hsv.S = def_S;
}

int main(void) {
  int32_t timer = -1;
  bool _pulsing = false;
  float tmp_V = 1.0f;
  bool pulsing_down = true;
  bool power = true;
  bool update = true;
  uint8_t led_c = 0;
  state_t state = {0};

  ADCSRA &= ~(1 << ADEN);  // ADC off
  DDRB |= _BV(ws2812_pin) | _BV(LED_PIN);
  PORTB &= ~_BV(LED_PIN);

  // Set up Timer/Counter0 (assumes 8MHz clock)
  TCCR0A = 0;                      // No compare matches
  TCCR0B = _BV(CS02) | _BV(CS00);  // Prescaler /1024
  // Set up INT0 interrupt on PB2
  MCUCR = MCUCR | 2 << ISC00;  // Interrupt on falling edge
  GIMSK = GIMSK | 1 << INT0;   // Enable INT0

  eeprom_read();
  uint16_t def_H = g_hsv.H;
  float def_S = g_hsv.S;

  update_leds();
  // set_sleep_mode(SLEEP_MODE_IDLE);
  setup_wdt();

  for (;;) {
    cli();
    while (g_state.events == 0) {
      sleep_enable();
      sei();
      sleep_cpu();
      sleep_disable();
      cli();
    }
    state.events = g_state.events;
    state.ir_data = g_state.ir_data;
    g_state.events = 0;
    sei();
    const uint8_t events = state.events;

    if (events & EV_IR) {
      PORTB |= _BV(LED_PIN);
      const uint16_t addr = state.ir_data & 0xFFFF;
      if (addr == IR_ADDR) {
        const uint16_t cmd = (state.ir_data >> 16) & 0xFFFF;
        update = true;
        switch (cmd) {
          case IR_CMD_PLUS:
            _pulsing = false;
            timer = -1;
            tmp_V += 0.1f;
            if (tmp_V > 1.0f) {
              tmp_V = 1.0f;
            }
            break;

          case IR_CMD_MINUS:
            _pulsing = false;
            timer = -1;
            tmp_V -= 0.1f;
            if (tmp_V < 0.0f) {
              tmp_V = 0.0f;
            }
            break;

          case IR_CMD_RIGHT:
            g_hsv.S += 0.01f;
            if (g_hsv.S > 1.0f) {
              g_hsv.S = 1.0f;
            }
            break;

          case IR_CMD_LEFT:
            g_hsv.S -= 0.01f;
            if (g_hsv.S < 0.0f) {
              g_hsv.S = 0.0f;
            }
            break;

          case IR_CMD_EQ:
            if (!(events & EV_REPEAT)) {
              _pulsing = false;
              timer = -1;
              if (power) {
                tmp_V = 0.0f;
              } else {
                tmp_V = 1.0f;
              }
              power ^= 1;
            } else {
              update = false;
            }
            break;

          case IR_CMD_CHPLUS:
            g_hsv.H += 2;
            if (g_hsv.H >= 360) {
              g_hsv.H = 0;
            }
            break;

          case IR_CMD_CHMINUS:
            g_hsv.H -= 2;
            if (g_hsv.H >= 360) {
              g_hsv.H = 360;
            }
            break;

          case IR_CMD_ZERO:
            if (!(events & EV_REPEAT)) {
              g_hsv.H = def_H;
              g_hsv.S = def_S;
              tmp_V = 1.0f;
            } else {
              update = false;
            }
            break;

          case IR_CMD_ONE:
            if (!(events & EV_REPEAT)) {
              if (timer > 0) {
                timer = -1;
                tmp_V = 1.0f;
                ack(2);
              } else {
                _pulsing = false;
                timer = TIMEOUT;
                tmp_V = 1.0f;
                ack(1);
                update = false;
              }
            }
            break;

          case IR_CMD_TWO:
            if (!(events & EV_REPEAT)) {
              if (!_pulsing) {
                _pulsing = true;
                timer = -1;
                tmp_V = 1.0f;
                pulsing_down = true;
                ack(1);
              } else {
                _pulsing = false;
                tmp_V = 1.0f;
                ack(2);
              }
            } else {
              update = false;
            }
            break;

          case IR_CMD_NINE:
            if (!(events & EV_REPEAT)) {
              eeprom_update_word(&eeprom_H, g_hsv.H);
              eeprom_update_float(&eeprom_S, g_hsv.S);
              def_S = g_hsv.S;
              def_H = g_hsv.H;
              ack(1);
            }
            update = false;
            break;

          default:
            update = false;
            break;
        }
      }
    }

    if (events & EV_TICK) {
      if (timer > 0) {
        timer--;
        if (timer == 0) {
          timer = -1;
          tmp_V = 0.0f;
          power = false;
          update = true;
        } else {
          tmp_V -= 1.0f / TIMEOUT;
          if (tmp_V < 0.0f) {
            tmp_V = 0.0f;
          }
          update = true;
        }
      }
      if (_pulsing) {
        if (pulsing_down) {
          tmp_V -= 0.02f;
          if (tmp_V < 0.2f) {
            tmp_V = 0.2f;
            pulsing_down = false;
          }
        } else {
          tmp_V += 0.02f;
          if (tmp_V > 1.0f) {
            tmp_V = 1.0f;
            pulsing_down = true;
          }
        }
        update = true;
      }

      // activity LED
      led_c = (led_c + 1) % 8;
      if (led_c > (8 - 2)) {
        PORTB |= _BV(LED_PIN);
      } else {
        PORTB &= ~_BV(LED_PIN);
      }
    }

    if (update) {
      g_hsv.V = tmp_V * tmp_V;
      update_leds();
      update = false;
    }
  }
}
