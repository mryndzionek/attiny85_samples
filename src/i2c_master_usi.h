#ifndef I2C_MASTER_USI_H_
#define I2C_MASTER_USI_H_

#include <stdbool.h>
#include <stdint.h>

void i2c_init(void);
bool i2c_write(uint8_t data);
uint8_t i2c_read(void);
uint8_t i2c_read_last(void);
bool i2c_start(uint8_t addr, int32_t readcount);
void i2c_stop(void);

#endif /* I2C_MASTER_USI_H_ */
