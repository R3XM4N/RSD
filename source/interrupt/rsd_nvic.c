#include "../../include/interrupt/rsd_nvic.h"

static uint8_t interrupt_enabled_flag = 0;

void nvic_enable_irq(const uint8_t interrupt_num){
    if (interrupt_num < 32){
        NVIC_ISER0 = (1u << interrupt_num);
    }
    else{
        NVIC_ISER1 = (1u << (interrupt_num - 32));
    }
}

void nvic_disable_irq(const uint8_t interrupt_num){
    if (interrupt_num < 32){
        NVIC_ICER0 = (1u << interrupt_num);
    }
    else{
        NVIC_ICER1 = (1u << (interrupt_num - 32));
    }
}

void nvic_set_pending(const uint8_t interrupt_num){
    if (interrupt_num < 32){
        NVIC_ISPR0 = (1u << interrupt_num);
    }
    else{
        NVIC_ISPR1 = (1u << (interrupt_num - 32));
    }
}

// void nvic_enable_interrupts(){
//     if (!interrupt_enabled_flag){
//         __asm__ volatile ("cpsie i" : : : "memory");
//         interrupt_enabled_flag = 1;
//     }
// }