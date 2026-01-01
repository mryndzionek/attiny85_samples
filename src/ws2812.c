#include <avr/eeprom.h>
#include <avr/io.h>
#include <math.h>
#include <stdbool.h>
#include <util/delay.h>

#include "light_ws2812.h"

#define NUM_LEDS (128)
#define BTN_PIN (4)

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

static CRGB_t leds[NUM_LEDS];

static int button_decode(bool s) {
  static bool deb = false;
  static uint16_t deb_c = 0;
  static int state = 0;
  static uint16_t c = 0;
  static int pk = 0;
  int press = 0;
  int ret = 0;

  switch (deb) {
    case false:
      if (s) {
        if (deb_c < 20) {
          deb_c++;
        } else {
          deb = true;
          press = 1;
        }
      } else {
        if (deb_c > 0) {
          deb_c--;
        }
      }
      break;

    case true:
      if (s) {
        if (deb_c < 20) {
          deb_c++;
        }
      } else {
        if (deb_c > 0) {
          deb_c--;
        } else {
          deb = false;
          press = -1;
        }
      }
      break;
  }

  switch (state) {
    case 0:  // idle
      if (press == 1) {
        state = 1;
      }
      break;

    case 1:  // short press?
      if (press == 0) {
        c++;
        if (c == 200) {
          state = 3;
          ret = 1;  // long press
        }
      } else if (press == -1) {
        state = 2;
        c = 0;
        pk++;
      }
      break;

    case 2:
      if (press == 0) {
        c++;
        if (c == 100) {
          ret = pk + 1;  // short presses
          pk = 0;
          state = 0;
        }
      } else if (press == 1) {
        state = 1;
        c = 0;
      }
      break;

    case 3:
      if (press == -1) {
        c = 0;
        state = 0;
        ret = -1;
      }
      break;
  }

  return ret;
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

static hsv_t g_hsv = {0, 1.0, 1.0};

static void update_leds(void) {
  CRGB_t rgb = hsv_to_rgb(g_hsv);
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i] = rgb;
  }
  ws2812_sendarray((uint8_t*)leds, sizeof(leds));
}

static inline void ack(void) {
  float v = g_hsv.V;
  g_hsv.V = 0.0f;
  update_leds();
  _delay_ms(100);
  g_hsv.V = v;
  update_leds();
}

int main(void) {
  bool lp = false;
  uint16_t c = 0;
  bool toggle = false;
  int32_t timer = -1;
  float pulsing = -1.0f;
  bool pulsing_down = true;
  uint8_t timer_50hz = 0;

  DDRB |= _BV(ws2812_pin);
  DDRB &= ~_BV(BTN_PIN);
  PORTB |= _BV(BTN_PIN);

  static uint16_t eeprom_hue EEMEM = 0;
  g_hsv.H = eeprom_read_word(&eeprom_hue);
  if (g_hsv.H > 360) {
    g_hsv.H = 0;
    eeprom_update_word(&eeprom_hue, g_hsv.H);
  }

  update_leds();

  for (;;) {
    _delay_ms(2);
    int r = button_decode((PINB & _BV(BTN_PIN)) == 0);
    if (r > 1) {
      r--;
      if (r != 4) {
        timer = -1;
      }
      if (r != 5) {
        pulsing = -1.0f;
        pulsing_down = true;
      }
      if (r == 1) {
        g_hsv.V -= 0.1;
        if (g_hsv.V < -0.05) {
          g_hsv.V = 1.0;
        }
      } else if (r == 2) {
        if (toggle) {
          g_hsv.V = 1.0;
        } else {
          g_hsv.V = 0.0;
        }
        toggle ^= 1;
      } else if (r == 3) {
        g_hsv = (hsv_t){0, 1.0, 1.0};
      } else if (r == 4) {
        if (timer > 0) {
          timer = -1;
        } else {
          timer = 90000;
          ack();
        }
      } else if (r == 5) {
        if (pulsing < 0.0f) {
          pulsing = 1.0f;
          pulsing_down = true;
        } else {
          pulsing = -1.0f;
          g_hsv.V = 1.0f;
        }
        ack();
      } else if (r == 6) {
        eeprom_update_word(&eeprom_hue, g_hsv.H);
        ack();
      }
    } else if (r == 1) {
      g_hsv.H++;
      if (g_hsv.H >= 360) {
        g_hsv.H -= 360;
      }
      lp = true;
    } else if (r == -1) {
      lp = false;
      c = 0;
    }
    if (++timer_50hz == 10) {
      timer_50hz = 0;
    }
    if (lp) {
      c++;
      if (c == 10) {
        c = 0;
        g_hsv.H++;
        if (g_hsv.H >= 360) {
          g_hsv.H -= 360;
        }
      }
    }
    if (timer > 0) {
      timer--;
      if (timer == 0) {
        timer = -1;
        g_hsv.V = 0.0f;
        toggle = true;
        update_leds();
      } else {
        if ((timer % 900) == 0) {
          g_hsv.V -= 0.01f;
          if (g_hsv.V < 0.0f) {
            g_hsv.V = 0.0f;
          }
          update_leds();
        }
      }
    }
    if ((pulsing >= 0.0f) && (timer_50hz == 0)) {
      if (pulsing_down) {
        pulsing -= 0.005f;
        if (pulsing < 0.2f) {
          pulsing = 0.2f;
          pulsing_down = false;
        }
      } else {
        pulsing += 0.005f;
        if (pulsing > 1.0f) {
          pulsing = 1.0f;
          pulsing_down = true;
        }
      }
      g_hsv.V = pulsing * pulsing;
      update_leds();
    }
    if ((r != 0) || (lp)) {
      update_leds();
    }
  }
}
