#ifndef RSD_INTERRUPT_H
#define RSD_INTERRUPT_H

#include <stdint.h>

#include "rsd_nvic.h"

#define VTOR                  (*(volatile uint32_t*)0xE000ED08u)
#define VECTOR_TABLE_ENTRIES  128

#define IO_IRQ_BANK0_NUM      21

void isr_io_bank0(void);

void irq_init(void);
void irq_register(const uint8_t irq_numer, void (*handler)(void));

#endif