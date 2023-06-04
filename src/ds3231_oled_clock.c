#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>

#include <u8g2.h>
#include <u8x8_avr.h>
#include "i2c_master_usi.h"

#define SSD1306_ADDR (0x78)

#define NUM_DIGITS (6)

#define DS3231_ADDR (0x68)
#define DS3231_TIME_CAL_REG (0x00)
#define DS3231_CONTROL_REG (0x0e)
#define DS3231_TEMP_REG_U (0x11)

#define DS3231_CTRL_INTCN (1UL << 2)
#define DS3231_CTRL_RS1 (1UL << 3)
#define DS3231_CTRL_RS2 (1UL << 4)
#define DS3231_CTRL_BBSQW (1UL << 6)

#define EV_DS3231_SQW _BV(0)

typedef struct
{
  uint8_t sec;
  uint8_t min;
  uint8_t hour;
  uint8_t mday;
  uint8_t mon;
  int16_t year;
  uint8_t wday;
  bool alarm;
} ds3231_time_t;

#if 0
// 7-segment font
static const uint8_t _font_tn[612] U8G2_FONT_SECTION("_font_tn") =
    "\13\1\4\4\5\5\1\2\6\23\36\0\1\35\0\35\0\0\0\0\0\2G \13\324\237~\370\377\377\377"
    "\377\3\60C\324\237\66\264IU\210\22r\202\320\20\23\204\210\220(TDH!S\205\354#\202\204\206"
    "\216\12\34\36\26\201C\15\35D\220\220\215\316\30:c\350L\261B\202\206\224;Q*\11)\25\244\224"
    "\10D\22\10\0\61,\324\237~\230\212\7B\36\4\371\232\333\36\4y \343\301\204\207yx\60\343A"
    "\20\67~\372\362\362\325\203 \17\202<\210\361`\2\1\62\61\324\237\66\264IU\210<A\322\4Q\22"
    "e\205\224/n{\20$\215\14D\22P\351\272 \10\207\230$\17\302\270\315\313\13\32~\66\255R\305"
    "\350\0\63\70\324\237\66\264IU\210<A\322\4Q\22e\205\224/n{\20$\215\14D\22P\351R"
    "$A\215\214\7A\334\370\351\313\213\16)w\242T\22R*H)\21\210$\20\0\64\66\324\237~\230"
    "\212\22Jh$!r\205\212\25\62U\310>\42Hh\210\221QA\220\4T\272\24IP#\343A\20"
    "\67~\372\362\362\325\203 \17\202<\210\361`\2\1\65\67\324\237\66\264I\25\12\71\71\304$\21\242E"
    "\206\32\267\71y\20C\214\6A\253t)\222\240F\306\203 n\374\364\345E\207\224;Q*\11)\25"
    "\244\224\10D\22\10\0\66\77\324\237\66\264I\25\12\71\71\304$\21\242E\206\32\267\71y\20C\214\6"
    "A\253t]\20$\241\206\30\31D\220\220\215\316\30:c\350L\261B\202\206\224;Q*\11)\25\244"
    "\224\10D\22\10\0\67.\324\237\66\264IU\210<A\322\4Q\22e\205\224/n{\20\344\201\214\7"
    "\23\36\336\214\7A\334\356k\17\202<\10\362@\304\203\11\4\0\70H\324\237\66\264IU\210\22r\202"
    "\320\20\23\204\210\220(TDH!S\205\354#\202\204\206\30\31\25\4I@\245\353\202 \11\65\304\310"
    " \202\204lt\306\320\31Cg\212\25\22\64\244\334\211RIH\251 \245D \222@\0\71@\324\237"
    "\66\264IU\210\22r\202\320\20\23\204\210\220(TDH!S\205\354#\202\204\206\30\31\25\4I@"
    "\245K\221\4\65\62\36\4q\343\247//:\244\334\211RIH\251 \245D \222@\0\0\0\0\4"
    "\377\377\0";
#else
// Predator font
static const uint8_t _font_tn[415] U8G2_FONT_SECTION("_font_tn") =
    "\13\1\5\2\5\6\1\3\6\22\42\0\2\42\0\42\0\0\0\0\0\1\202 \12Tl\372\363\377\377:"
    "\0\60\31Tl\372\203f\366>;\231\232\231\232\371\230\231\232\231\232\371lgv\13\61\36Tl\372#"
    "f\366\134\230\312\200\310\300>d\244d\244d>\37\235\34\320\207\310\354\20\0\62/Tl\372#F\4"
    "F\4F\4F\4F\4Fd\205\251\14\210\14\354C\206\244\211\210\305d\311H\5\345cfjfj"
    "\346\223\230\34\320g\35\63\30Tl\372\363\252\334\300>_\310\324\314\324\314'\61\71 \237\331-\0\64"
    "\42Tl\372\363\252\334\300>dH\232\210XL\226\214TP>f\246f\246f>\211\311\1\371\314n"
    "\1\65$Tl\372\203f\366V\346\200>x\244d\244d>xP\224NV\214\226JZ\210>\366\344"
    "\200|f\267\0\66$Tl\372\363\363!i\42b\61Y\62RA\371\230AQ:Y\61Z*i!"
    "\372\330\223\3\372\20\231\35\2\67\37Tl\372\203f\366V\346\200>x\244d\244d>\231\314H\311H"
    "}\354\311\1\371\314n\1\70\62Tl\372#F\4F\4F\4F\4F\4Fd\205\251\14\210\14\354"
    "C\206\244\211\210\305d\311H\5\345cfjfj\346\223\230\34\320\207\310\354\20\0\71=Tl\372#"
    "F\4F\4F\4F\4F\4Fd\205\251\14\210\14\354C\206\244\211\210\305d\311H\5\345c\6E"
    "\351d\305h\251\244\205\350cO\16\310G\4F\4F\4F\4F\4\1\0\0\0\4\377\377\0";
#endif

static u8g2_t u8g2;
static volatile uint_fast8_t events;
static ds3231_time_t ds3231_time;
static char hour[] = "      ";
static char date[] = "__.__.__";
static char temp_str[] = "+__.__";
static uint16_t temp;

static void num_to_str(uint32_t num, char *const buf, uint8_t len)
{
  uint32_t k = num;

  for (uint8_t j = 0; j < len; j++)
  {
    buf[len - 1 - j] = '0' + (k % 10);
    k /= 10;
  }
}

inline static uint8_t bcdtodec(const uint8_t val)
{
  return ((val / 16 * 10) + (val % 16));
}

inline static uint8_t dectobcd(const uint8_t val)
{
  return ((val / 10 * 16) + (val % 10));
}

static void ds3231_init(void)
{
  i2c_start(DS3231_ADDR, 0);
  i2c_write(DS3231_CONTROL_REG);
  i2c_start(DS3231_ADDR, 1);
  uint8_t data = i2c_read();
  i2c_stop();

  // Enable 1Hz SQW output
  data &= ~(DS3231_CTRL_INTCN | DS3231_CTRL_RS1 | DS3231_CTRL_RS2);
  data |= DS3231_CTRL_BBSQW;

  i2c_start(DS3231_ADDR, 0);
  i2c_write(DS3231_CONTROL_REG);
  i2c_write(data);
  i2c_stop();
}

static void ds3231_set_time(void)
{
  uint8_t data[7];

  data[0] = dectobcd(0);
  data[1] = dectobcd(53);
  data[2] = dectobcd(8);
  data[3] = dectobcd(3);
  data[4] = dectobcd(6);
  data[5] = dectobcd(6) + 0x80;
  data[6] = dectobcd(23);

  i2c_start(DS3231_ADDR, 0);
  i2c_write(DS3231_TIME_CAL_REG);
  for (uint8_t i = 0; i < 7; i++)
  {
    i2c_write(data[i]);
  }
  i2c_stop();
}

static void ds3231_get_time(void)
{
  i2c_start(DS3231_ADDR, 0);
  i2c_write(DS3231_TIME_CAL_REG);
  i2c_start(DS3231_ADDR, 7);

  ds3231_time.sec = bcdtodec(i2c_read());
  ds3231_time.min = bcdtodec(i2c_read());
  ds3231_time.hour = bcdtodec(i2c_read());
  ds3231_time.wday = bcdtodec(i2c_read());
  ds3231_time.mday = bcdtodec(i2c_read());
  ds3231_time.mon = bcdtodec(i2c_read() & (0x7F));
  ds3231_time.year = bcdtodec(i2c_read());

  i2c_stop();
}

static void ds3231_get_temp(void)
{
  i2c_start(DS3231_ADDR, 0);
  i2c_write(DS3231_TEMP_REG_U);
  i2c_start(DS3231_ADDR, 2);
  temp = i2c_read() << 2;
  temp |= i2c_read() >> 6;
  i2c_stop();
}

static void update_state(void)
{
  num_to_str(ds3231_time.hour, hour, 2);
  num_to_str(ds3231_time.min, &hour[2], 2);
  num_to_str(ds3231_time.sec, &hour[4], 2);

  num_to_str(ds3231_time.mday, date, 2);
  num_to_str(ds3231_time.mon, &date[3], 2);
  num_to_str(ds3231_time.year, &date[6], 2);

  num_to_str((temp & (~0x80)) >> 2, &temp_str[1], 2);
  num_to_str((temp & 0x3) * 25, &temp_str[4], 2);
  if (temp & 0x80)
  {
    temp_str[0] = '-';
  }
  else
  {
    temp_str[0] = '+';
  }
}

ISR(PCINT0_vect)
{
  if (!(PINB & _BV(PB4)))
  {
    events |= EV_DS3231_SQW;
  }
}

int main(void)
{
  DDRB |= _BV(PB3);
  PORTB &= ~_BV(PB3);

  power_all_disable();
  power_usi_enable();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  // Enable PCINT4 (PB4)
  GIMSK |= _BV(PCIE);
  PCMSK |= _BV(PCINT4);

  u8g2_Setup_ssd1306_i2c_128x64_noname_1(&u8g2, U8G2_R0, u8x8_byte_avr_hw_i2c, u8x8_avr_delay);
  u8g2_SetI2CAddress(&u8g2, SSD1306_ADDR >> 1);
  u8g2_InitDisplay(&u8g2);
  u8g2_SetPowerSave(&u8g2, 0);
  u8g2_SetContrast(&u8g2, 255);
  u8g2_SetFlipMode(&u8g2, 1);
  u8g2_ClearDisplay(&u8g2);

  // ds3231_set_time();
  ds3231_init();
  ds3231_get_time();
  ds3231_get_temp();
  update_state();

  sei();

  while (1)
  {
    sleep_mode();
    if (events & EV_DS3231_SQW)
    {
      PORTB |= _BV(PB3);
      ds3231_time.sec++;
      if (ds3231_time.sec == 60)
      {
        ds3231_time.sec = 0;
        ds3231_time.min++;
        ds3231_get_temp();
        if (ds3231_time.min == 60)
        {
          ds3231_get_time();
        }
      }
      update_state();

      u8g2_FirstPage(&u8g2);
      do
      {
        u8g2_SetFont(&u8g2, _font_tn);
        u8g2_DrawStr(&u8g2, 3, 50, hour);

        u8g2_SetFont(&u8g2, u8g2_font_t0_11_tn);
        u8g2_DrawStr(&u8g2, 75, 10, date);
        u8g2_DrawStr(&u8g2, 0, 10, temp_str);

        for (uint8_t i = 0; i < 5; i++)
        {
          u8g2_DrawHLine(&u8g2, 4 + (i * 30) - 1, 59, 2);
          u8g2_DrawHLine(&u8g2, 4 + (i * 30) - 1, 60, 2);
          u8g2_DrawHLine(&u8g2, 4 + (i * 30) - 1, 61, 2);
        }

        for (uint8_t i = 0; i < 12; i++)
        {
          u8g2_DrawHLine(&u8g2, 4 + (i * 10), 60, 1);
          u8g2_DrawHLine(&u8g2, 4 + (i * 10), 61, 1);
        }

        u8g2_DrawHLine(&u8g2, 4, 62, (ds3231_time.sec + 1) * 2);
        u8g2_DrawHLine(&u8g2, 4, 63, (ds3231_time.sec + 1) * 2);
      } while (u8g2_NextPage(&u8g2));
      PORTB ^= _BV(PB3);
    }
    events = 0;
  }

  return 0;
}
