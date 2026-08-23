#ifndef RSD_RESET_H
#define RSD_RESET_H

#include <stdint.h>

/// Base then offset for specific ones
#define RESETS_BASE     0x4000C000
#define RESETS_RESET    (*(volatile uint32_t*)(RESETS_BASE + 0x0)) // bits in a row of peripheral reset control
#define RESETS_DONE     (*(volatile uint32_t*)(RESETS_BASE + 0x8)) // bits in a row of peripheral read reset state (out of reset?)

/// @brief Awaits (waits for) a reset to be complete on a bit position
/// @param bit_pos The position of the bit (example: gpio waits for bit pos 5)
void reset_await(uint8_t bit_pos);

#endif