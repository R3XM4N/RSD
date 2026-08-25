#ifndef RSD_GPIO_H
#define RSD_GPIO_H

#include <stdint.h>

/// In a one bit sets one each
#define SIO_BASE        0xD0000000u
#define GPIO_OUT_SET    (*(volatile uint32_t*)(SIO_BASE + 0x14))
#define GPIO_OUT_CLR    (*(volatile uint32_t*)(SIO_BASE + 0x18))
#define GPIO_OE_SET     (*(volatile uint32_t*)(SIO_BASE + 0x24))
#define GPIO_OE_CLR     (*(volatile uint32_t*)(SIO_BASE + 0x28))

/// @brief Sets the pin into a gpio (having 0/1 logic only)
/// @param gpio_pin Identifier of the desired PIN
void gpio_enable_out(const uint8_t gpio_pin);
/// @brief Disables the pin away from gpio
/// @param gpio_pin Identifier of the desired PIN
void gpio_disable_out(const uint8_t gpio_pin);
/// @brief Sets the pin to high (1)
/// @param gpio_pin Identifier of the desired PIN
void gpio_write(const uint8_t gpio_pin);
/// @brief Sets the pin to low (0)
/// @param gpio_pin Identifier of the desired PIN
void gpio_clear(const uint8_t gpio_pin);
/// @brief Enables falling edge itnerrupt on a pin
/// @param gpio_pin Identifier of the desired PIN
void gpio_enable_fall_irq(const  uint8_t gpio_pin);

static inline uint8_t gpio_read(const uint8_t gpio_pin){ //test
    return ((*(volatile uint32_t*)(SIO_BASE + 0x004)) & (1u << gpio_pin)) ? 1 : 0;
}

#endif