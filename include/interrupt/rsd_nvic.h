#ifndef RSD_NVIC_H
#define RSD_NVIC_H

#include <stdint.h>

#define NVIC_ISER0 (*(volatile uint32_t*)(0xE000E100u)) // Interrupt set enable
#define NVIC_ISER1 (*(volatile uint32_t*)(0xE000E104u))
#define NVIC_ICER0 (*(volatile uint32_t*)(0xE000E180u)) // Interrupt clear enable
#define NVIC_ICER1 (*(volatile uint32_t*)(0xE000E184u))
#define NVIC_ICPR0 (*(volatile uint32_t*)(0xE000E280u)) // Interrupt clear pending
#define NVIC_ICPR1 (*(volatile uint32_t*)(0xE000E284u))

// For setting by hand
#define NVIC_ISPR0 (*(volatile uint32_t*)(0xE000E200u))
#define NVIC_ISPR1 (*(volatile uint32_t*)(0xE000E204u))

/// @brief  Enables interruptert request signal
void nvic_enable_irq(const uint8_t interrupt_num);
/// @brief  Disables hw irq channel 
void nvic_disable_irq(const uint8_t interrupt_num);
// /// @brief Enables interrupts
// void nvic_enable_interrupts();
void nvic_set_pending(const uint8_t interrupt_num);

#endif