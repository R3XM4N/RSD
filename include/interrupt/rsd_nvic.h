#ifndef RSD_NVIC_H
#define RSD_NVIC_H

#include <stdint.h>

// PBB BASE
#define NVIC_BASE      0xE0000000u
#define NVIC_ISER      (*(volatile uint32_t*)(NVIC_BASE + 0xE100u)) // Interrupt set enable
#define NVIC_ICER      (*(volatile uint32_t*)(NVIC_BASE + 0xE180u)) // Interrupt clear enable
#define NVIC_ICPR      (*(volatile uint32_t*)(NVIC_BASE + 0xE280u)) // Interrupt clear pending

/// @brief  Enables interruptert request signal
void nvic_enable_irq(uint8_t interrupt_num);
/// @brief  Disables hw irq channel 
void nvic_disable_irq(uint8_t interrupt_num);
// /// @brief Enables interrupts
// void nvic_enable_interrupts();

#endif