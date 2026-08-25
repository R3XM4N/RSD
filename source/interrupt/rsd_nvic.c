#include "../../include/interrupt/rsd_nvic.h"

static uint8_t interrupt_enabled_flag = 0;

void nvic_enable_irq(uint8_t interrupt_num){
    // nvic_enable_interrupts();
    NVIC_ISER = (1u << interrupt_num);
}

void nvic_disable_irq(uint8_t interrupt_num){
    NVIC_ICER = (1u << interrupt_num);
}

// void nvic_enable_interrupts(){
//     if (!interrupt_enabled_flag){
//         __asm__ volatile ("cpsie i" : : : "memory");
//         interrupt_enabled_flag = 1;
//     }
// }