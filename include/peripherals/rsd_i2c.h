#ifndef RSD_I2C_H
#define RSD_I2C_H

#include <stdint.h>

void i2c0_init(const uint8_t sda_pin, const uint8_t scl_pin, const uint32_t baud_rate_hz);
void i2c0_write_blocking(const uint8_t target_7b, const uint8_t* data, const uint32_t data_byte_count, uint8_t send_stop);

#endif