#include "../include/rsd_sys_tick.h"

static volatile uint32_t ms_ticks = 0;

void sys_tick_init(uint32_t cpu_hz){
    SYST_RVR = (cpu_hz / 1000) - 1; //sets val this for 1ms
    SYST_CVR = 0; // reset
    SYST_CSR = (1u << 0) | (1u << 1) | (1u << 2); // enable | tickinit | clksource
}

void delay_ms(uint32_t ms){
    uint32_t start = ms_ticks;
    while ((ms_ticks - start) < ms){
        // __asm__("wfi"); //wait till interupt
    }
}


void SysTick_Handler(void){
    ms_ticks++;
}