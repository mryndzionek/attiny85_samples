#include <stdlib.h>

#include <avr/io.h>
#include <util/delay.h>

#include <u8g2.h>
#include <u8x8_avr.h>

#define SSD1306_ADDR (0x78)
#define NUM_DIGITS (6)

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

int main(void)
{
  DDRB |= _BV(PB3);

  u8g2_Setup_ssd1306_i2c_128x64_noname_1(&u8g2, U8G2_R0, u8x8_byte_avr_hw_i2c, u8x8_avr_delay);
  u8g2_SetI2CAddress(&u8g2, SSD1306_ADDR >> 1);
  u8g2_InitDisplay(&u8g2);
  u8g2_SetPowerSave(&u8g2, 0);
  u8g2_SetContrast(&u8g2, 0xff);
  u8g2_SetFlipMode(&u8g2, 1);
  u8g2_ClearDisplay(&u8g2);

  uint32_t i = 0;
  uint32_t j;
  const char *str;

  while (1)
  {
    PORTB |= _BV(PB3);
    j = rand() % 1000000;
    u8g2_FirstPage(&u8g2);
    do
    {
      str = num_to_str(i);
      u8g2_SetFont(&u8g2, u8g2_font_tenstamps_mn);
      u8g2_DrawStr(&u8g2, 0, 15, str);
      str = num_to_str(j);
      u8g2_SetFont(&u8g2, u8g2_font_7_Seg_41x21_mn);
      u8g2_DrawStr(&u8g2, 0, 18, str);
    } while (u8g2_NextPage(&u8g2));

    i++;
    PORTB &= ~_BV(PB3);
    _delay_ms(800);
  }

  return 0;
}
