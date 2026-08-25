#include "../../include/interrupt/rsd_interrupt.h"

#include "../../include/peripherals/rsd_pin.h"
#include "../../include/os/rsd_os.h"

__attribute__((aligned(256))) static uint32_t ram_vector_table[48];
void irq_init(void){
    uint32_t* flash_vector = (uint32_t*)VTOR;
    for (uint8_t i = 0; i < 48; i++){
        ram_vector_table[i] = flash_vector[i];
    }
    VTOR = (uint32_t)ram_vector_table;
    __asm__ volatile ("dsb \n isb" : : : "memory");
    __asm__ volatile ("cpsie i" : : : "memory");
}

void irq_register(const uint8_t irq_numer, void (*handler)(void)){
    ram_vector_table[16 + irq_numer] = (uint32_t)handler;
    NVIC_ISER = (1u << irq_numer);
}

extern volatile uint32_t ms_ticks;
static uint32_t gpio_last_press_time = 0;

#define IO_INTR(idx)     (*(volatile uint32_t*)(IO_BANK0_BASE + 0x0F0 + ((idx) * 4))) // INTR (Interrupt Status / Clear) starts at offset 0x0F0
#define PROC0_INTS(idx)  (*(volatile uint32_t*)(IO_BANK0_BASE + 0x120 + ((idx) * 4))) // PROC0_INTS (Core 0 Status)

void isr_io_bank0(void){
    for (uint8_t reg_idx = 0; reg_idx < 4; reg_idx++){
        uint32_t active_interrupts = PROC0_INTS(reg_idx);

        if (active_interrupts == 0){ continue;}
        
        for (uint8_t pin_idx = 0; pin_idx < 8; pin_idx++){
            uint8_t gpio_pin = (reg_idx * 8) + pin_idx;
            if (gpio_pin > 29) break;

            uint8_t bit_shift_amm = pin_idx * 4;
            uint32_t fall_edge_mask = (1u << (bit_shift_amm + 2));

            if (active_interrupts & fall_edge_mask){
                IO_INTR(reg_idx) = fall_edge_mask; // ackno and clear
                // handle interrupt(gpio_pin);
                test_call();
            }
            
        }
        
    }
    
}