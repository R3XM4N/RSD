#ifndef RSD_INTERRUPT_H
#define RSD_INTERRUPT_H

#include <stdint.h>

#include "rsd_nvic.h"

#define VTOR      (*(volatile uint32_t*)0xE000ED08u)

void isr_io_bank0(void);



__attribute__((aligned(256))) static uint32_t ram_vector_table[48];
void irq_init(void);
void irq_register();


#endif