// #include "pico/stdlib.h"
#include <stdint.h>

/// In a one bit sets one each
#define SIO_BASE        0xD0000000u
#define GPIO_OUT_SET    (*(volatile uint32_t*)(SIO_BASE + 0x14))
#define GPIO_OUT_CLR    (*(volatile uint32_t*)(SIO_BASE + 0x18))
#define GPIO_OE_SET     (*(volatile uint32_t*)(SIO_BASE + 0x24))

///jumped to the pin of the base
#define IO_BANK0_BASE   0x40014000u

/// Base then offset for specific ones
#define RESETS_BASE     0x4000C000
#define RESETS_RESET    (*(volatile uint32_t*)(RESETS_BASE + 0x0)) // bits in a row of peripheral reset control
#define RESETS_DONE     (*(volatile uint32_t*)(RESETS_BASE + 0x8)) // bits in a row of peripheral read reset state (out of reset?)
