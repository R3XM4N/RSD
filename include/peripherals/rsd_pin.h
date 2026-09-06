#ifndef RSD_PIN_H
#define RSD_PIN_H

#include <stdint.h>

#define REG(base, off) (*(volatile uint32_t*)((base) + (off))) // Universal BASE + OFFEST register locator
/// What peripheral owns what pin basically. (It's a base register you offset from here). Global cuz no one can stop me 
#define IO_BANK0_BASE   0x40028000u
#define SIO_BASE        0xD0000000u

/// @brief Sets the pin into the desired funscel
/// @param pin gpio_pin_id
/// @param funcsel_id id of the funcsel
static inline void gpio_set_funcsel(const uint32_t pin, const uint32_t funcsel_id){
    REG(IO_BANK0_BASE, 8u * pin + 4u) = funcsel_id;
}

#endif