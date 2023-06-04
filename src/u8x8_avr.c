#include <util/delay.h>

#include "u8x8_avr.h"
#include "i2c_master_usi.h"

#define P_CPU_NS (1000000000UL / F_CPU)

uint8_t u8x8_byte_avr_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t *data;
  switch (msg)
  {
  case U8X8_MSG_BYTE_SEND:
    data = (uint8_t *)arg_ptr;
    while (arg_int--)
      i2c_write(*data++);
    break;
  case U8X8_MSG_BYTE_INIT:
    i2c_init();
    break;
  case U8X8_MSG_BYTE_SET_DC:
    /* ignored for i2c */
    break;
  case U8X8_MSG_BYTE_START_TRANSFER:
    i2c_start(u8x8_GetI2CAddress(u8x8), 0);
    break;
  case U8X8_MSG_BYTE_END_TRANSFER:
    i2c_stop();
    break;
  default:
    return 0;
  }
  return 1;
}

uint8_t u8x8_avr_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t cycles;

  switch (msg)
  {
  case U8X8_MSG_DELAY_NANO: // delay arg_int * 1 nano second
    // At 20Mhz, each cycle is 50ns, the call itself is slower.
    break;
  case U8X8_MSG_DELAY_100NANO: // delay arg_int * 100 nano seconds
// Approximate best case values...
#define CALL_CYCLES 26UL
#define CALC_CYCLES 4UL
#define RETURN_CYCLES 4UL
#define CYCLES_PER_LOOP 4UL

    cycles = (100UL * arg_int) / (P_CPU_NS * CYCLES_PER_LOOP);

    if (cycles > CALL_CYCLES + RETURN_CYCLES + CALC_CYCLES)
      break;

    __asm__ __volatile__(
        "1: sbiw %0,1"
        "\n\t" // 2 cycles
        "brne 1b"
        : "=w"(cycles)
        : "0"(cycles) // 2 cycles
    );
    break;
  case U8X8_MSG_DELAY_10MICRO: // delay arg_int * 10 micro seconds
    while (arg_int--)
      _delay_us(10);
    break;
  case U8X8_MSG_DELAY_MILLI: // delay arg_int * 1 milli second
    while (arg_int--)
      _delay_ms(1);
    break;
  default:
    return 0;
  }

  return 1;
}
