#ifndef RSD_SYS_TICK_H
#define RSD_SYS_TICK_H

#include <stdint.h>

// bit 0 - enable
// bit 1 - interupt on 0
// bit 2 - use cpu clock directly?
#define SYST_CSR  (*(volatile uint32_t*)(0xE000E010)) // Control & Status (bits)
#define SYST_RVR  (*(volatile uint32_t*)(0xE000E014)) // Reload Value aka starting number (24 bit down counter so)
#define SYST_CVR  (*(volatile uint32_t*)(0xE000E018)) // Current Value

void isr_systick(void);
void sys_tick_init(uint32_t cpu_hz);
void delay_ms(uint32_t ms);

#endif