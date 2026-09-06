#ifndef RSD_PIN_H
#define RSD_PIN_H

#include <stdint.h>

#define REG(base, off) (*(volatile uint32_t*)((base) + (off))) // Universal BASE + OFFEST register locator
/// What peripheral owns what pin basically. (It's a base register you offset from here). Global cuz no one can stop me 
#define IO_BANK0_BASE   0x40028000u
#define SIO_BASE        0xD0000000u

// funcsel code defines
typedef enum{
    F11_FUNCSEL     = 11,
    USB_FUNCSEL     = 10,
    F9_FUNCSEL      = 9,
    PIO2_FUNCSEL    = 8,
    PIO1_FUNCSEL    = 7,
    PIO0_FUNCSEL    = 6,
    SIO_FUNCSEL     = 5,
    PWM_FUNCSEL     = 4,
    I2C_FUNCSEL     = 3,
    UART_FUNCSEL    = 2,
    SPIO_FUNCSEL    = 1,
    HSTX_FUNCSEL    = 0
}funcsel;
#define GPIO_FUNCSEL SIO_FUNCSEL

typedef enum{
    IO_BANK0 = 6
}reset_bit;

/// @brief Sets the pin into the desired funscel
/// @param pin gpio_pin_id
/// @param funcsel_id id of the funcsel
void gpio_set_funcsel(const uint8_t pin, const funcsel funcsel_id);

#endif