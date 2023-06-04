#include "i2c_master_usi.h"

#include <avr/io.h>
#include <util/delay.h>

#define TWI_FAST_MODE

#ifdef TWI_FAST_MODE               // TWI FAST mode timing limits. SCL = 100-400kHz
#define DELAY_T2TWI (_delay_us(2)) // >1.3us
#define DELAY_T4TWI (_delay_us(1)) // >0.6us
#else                              // TWI STANDARD mode timing limits. SCL <= 100kHz
#define DELAY_T2TWI (_delay_us(5)) // >4.7us
#define DELAY_T4TWI (_delay_us(4)) // >4.0us
#endif

#define PORT_USI PORTB
#define PORT_USI_CL PORTB
#define PIN_USI_SDA PB0
#define PIN_USI_SCL PB2
#define DDR_USI DDRB
#define DDR_USI_CL DDRB
#define TWI_NACK_BIT (0)
#define PIN_USI_CL PINB

static unsigned char const USISR_8bit = 1 << USISIF | 1 << USIOIF | 1 << USIPF | 1 << USIDC | 0x0 << USICNT0;
static unsigned char const USISR_1bit = 1 << USISIF | 1 << USIOIF | 1 << USIPF | 1 << USIDC | 0xE << USICNT0;

static int32_t I2Ccount;

static uint8_t i2c_transfer(uint8_t data)
{
  USISR = data;                                    // Set USISR according to data.
                                                   // Prepare clocking.
  data = 0 << USISIE | 0 << USIOIE |               // Interrupts disabled
         1 << USIWM1 | 0 << USIWM0 |               // Set USI in Two-wire mode.
         1 << USICS1 | 0 << USICS0 | 1 << USICLK | // Software clock strobe as source.
         1 << USITC;                               // Toggle Clock Port.
  do
  {
    DELAY_T2TWI;
    USICR = data; // Generate positive SCL edge.
    while (!(PIN_USI_CL & 1 << PIN_USI_SCL))
      ; // Wait for SCL to go high.
    DELAY_T4TWI;
    USICR = data;                   // Generate negative SCL edge.
  } while (!(USISR & 1 << USIOIF)); // Check for transfer complete.

  DELAY_T2TWI;
  data = USIDR;                  // Read out data.
  USIDR = 0xFF;                  // Release SDA.
  DDR_USI |= (1 << PIN_USI_SDA); // Enable SDA as output.

  return data;
}

void i2c_init(void)
{
  PORT_USI |= 1 << PIN_USI_SDA;    // Enable pullup on SDA.
  PORT_USI_CL |= 1 << PIN_USI_SCL; // Enable pullup on SCL.

  DDR_USI_CL |= 1 << PIN_USI_SCL; // Enable SCL as output.
  DDR_USI |= 1 << PIN_USI_SDA;    // Enable SDA as output.

  USIDR = 0xFF;                                     // Preload data register with data.
  USICR = 0 << USISIE | 0 << USIOIE |               // Disable Interrupts.
          1 << USIWM1 | 0 << USIWM0 |               // Set USI in Two-wire mode.
          1 << USICS1 | 0 << USICS0 | 1 << USICLK | // Software stobe as counter clock source
          0 << USITC;
  USISR = 1 << USISIF | 1 << USIOIF | 1 << USIPF | 1 << USIDC | // Clear flags,
          0x0 << USICNT0;
}

uint8_t i2c_read(void)
{
  if ((I2Ccount != 0) && (I2Ccount != -1))
    I2Ccount--;

  /* Read a byte */
  DDR_USI &= ~(1 << PIN_USI_SDA); // Enable SDA as input.
  uint8_t data = i2c_transfer(USISR_8bit);

  /* Prepare to generate ACK (or NACK in case of End Of Transmission) */
  if (I2Ccount == 0)
    USIDR = 0xFF;
  else
    USIDR = 0x00;
  i2c_transfer(USISR_1bit); // Generate ACK/NACK.

  return data; // Read successfully completed
}

uint8_t i2c_read_last(void)
{
  I2Ccount = 0;
  return i2c_read();
}

bool i2c_write(uint8_t data)
{
  PORT_USI_CL &= ~(1 << PIN_USI_SCL);
  USIDR = data;
  i2c_transfer(USISR_8bit);

  /* Clock and verify (N)ACK from slave */
  DDR_USI &= ~(1 << PIN_USI_SDA);
  if (i2c_transfer(USISR_1bit) & 1 << TWI_NACK_BIT)
    return false;

  return true;
}

bool i2c_start(uint8_t addr, int32_t readcount)
{
  if (readcount != 0)
  {
    I2Ccount = readcount;
    readcount = 1;
  }
  uint8_t addressRW = addr << 1 | readcount;

  /* Release SCL to ensure that (repeated) Start can be performed */
  PORT_USI_CL |= 1 << PIN_USI_SCL; // Release SCL.
  while (!(PIN_USI_CL & 1 << PIN_USI_SCL))
    ; // Verify that SCL becomes high.
#ifdef TWI_FAST_MODE
  DELAY_T4TWI;
#else
  DELAY_T2TWI;
#endif

  /* Generate Start Condition */
  PORT_USI &= ~(1 << PIN_USI_SDA); // Force SDA LOW.
  DELAY_T4TWI;
  PORT_USI_CL &= ~(1 << PIN_USI_SCL); // Pull SCL LOW.
  PORT_USI |= 1 << PIN_USI_SDA;       // Release SDA.

  if (!(USISR & 1 << USISIF))
    return false;

  /*Write address */
  PORT_USI_CL &= ~(1 << PIN_USI_SCL); // Pull SCL LOW.
  USIDR = addressRW;                  // Setup data.
  i2c_transfer(USISR_8bit);           // Send 8 bits on bus.

  /* Clock and verify (N)ACK from slave */
  DDR_USI &= ~(1 << PIN_USI_SDA);
  if (i2c_transfer(USISR_1bit) & (1 << TWI_NACK_BIT))
    return false; // No ACK

  return true;
}

void i2c_stop(void)
{
  PORT_USI &= ~(1 << PIN_USI_SDA); // Pull SDA low.
  PORT_USI_CL |= 1 << PIN_USI_SCL; // Release SCL.
  while (!(PIN_USI_CL & 1 << PIN_USI_SCL))
    ; // Wait for SCL to go high.
  DELAY_T4TWI;
  PORT_USI |= 1 << PIN_USI_SDA; // Release SDA.
  DELAY_T2TWI;
}
