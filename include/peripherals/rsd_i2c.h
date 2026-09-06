#ifndef RSD_I2C_H
#define RSD_I2C_H

#include <stdint.h>

/// @brief Avaible i2cs
typedef enum{
    I2C_INSTANCE_0 = 0,
    I2C_INSTANCE_1 = 1
}i2c_instance_t;

/// @brief Initiates i2c lane and it's pins
/// @param instance which i2c instance is to be initiated
/// @param sda_pin sda pin of the i2c
/// @param scl_pin scl pin of the i2c
/// @param baud_rate_hz baud rate that is redudant rn hardcoded as 100kHz aka not supported at the moment 
void i2c_init(const i2c_instance_t instance, const uint8_t sda_pin, const uint8_t scl_pin, const uint32_t baud_rate_hz);

/// @brief Writes on i2c in a blocking pattern (doesn't return till the end aka non-async)
/// @param instance which i2c instance is to be used
/// @param target_7b the 7bit identification of the target slave/device 
/// @param data pointer to the data to be sent (Byte array pointer)
/// @param data_byte_count data byte count 
/// @param send_stop flag if it's the end of the transfer with this device (issue stop after last byte)
/// @return returns based on the events in the line (error codes etc) returns 1 on all ack 0 on nack
uint8_t i2c_write_blocking(const i2c_instance_t instance, const  uint8_t target_7b, const uint8_t* data, const uint32_t data_byte_count, const uint8_t send_stop);

/// @brief sweeps 7bit adress range until finds a valid target
/// @param instance which i2c instance is to be used
/// @return The FIRST valid adress it finds
uint8_t i2c_addr_sweep(const i2c_instance_t instance);

/// @brief checks and returns which i2c instances are active ina single byte based on byte position 
/// @return returns bits of a i2c state (bit 0 = i2c0 state, bit 1 = i2c1 state)
uint8_t i2c_get_active();

#endif