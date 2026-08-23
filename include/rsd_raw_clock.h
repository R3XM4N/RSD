#ifndef RAW_CLOCK_H
#define RAW_CLOCK_H

#include <stdint.h>

#define PLL_SYS_BASE        0x40028000u
#define PLL_SYS_CS          (*(volatile uint32_t*)(PLL_SYS_BASE + 0x00)) // Control/status
#define PLL_SYS_FBDIV_INT   (*(volatile uint32_t*)(PLL_SYS_BASE + 0x08)) // Feedback divider
#define PLL_SYS_PRIM        (*(volatile uint32_t*)(PLL_SYS_BASE + 0x0C)) // Post-dividers

/// @brief Returns sys cpu ferquency exists to not hard code fun.
uint32_t clocks_get_sys_hz(void);

#endif