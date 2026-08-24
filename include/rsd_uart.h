#ifndef RSD_UART_TICK_H
#define RSD_UART_TICK_H

#include <stdint.h>

#define UART0_BASE   0x40034000u
#define UART0_DR     (*(volatile uint32_t*)(UART0_BASE + 0x000)) // Data register
#define UART0_FR     (*(volatile uint32_t*)(UART0_BASE + 0x018)) // Flag register
#define UART0_IBRD   (*(volatile uint32_t*)(UART0_BASE + 0x024)) // Integer baud divisor
#define UART0_FBRD   (*(volatile uint32_t*)(UART0_BASE + 0x028)) // Fractional baud divisor
#define UART0_LCR_H  (*(volatile uint32_t*)(UART0_BASE + 0x02C)) // Line control
#define UART0_CR     (*(volatile uint32_t*)(UART0_BASE + 0x030)) // Control register
#define UART0_IMSC   (*(volatile uint32_t*)(UART0_BASE + 0x038)) // Interrupt Mask Set/Clear
#define UART0_MIS    (*(volatile uint32_t*)(UART0_BASE + 0x040)) // Masked Interrupt Status
#define UART0_ICR    (*(volatile uint32_t*)(UART0_BASE + 0x044)) // Interrupt Clear Register

#define UART1_BASE   0x40038000u
#define UART1_DR     (*(volatile uint32_t*)(UART1_BASE + 0x000)) // Data register
#define UART1_FR     (*(volatile uint32_t*)(UART1_BASE + 0x018)) // Flag register
#define UART1_IBRD   (*(volatile uint32_t*)(UART1_BASE + 0x024)) // Integer baud divisor
#define UART1_FBRD   (*(volatile uint32_t*)(UART1_BASE + 0x028)) // Fractional baud divisor
#define UART1_LCR_H  (*(volatile uint32_t*)(UART1_BASE + 0x02C)) // Line control
#define UART1_CR     (*(volatile uint32_t*)(UART1_BASE + 0x030)) // Control register
#define UART1_IMSC   (*(volatile uint32_t*)(UART1_BASE + 0x038)) // Interrupt Mask Set/Clear
#define UART1_MIS    (*(volatile uint32_t*)(UART1_BASE + 0x040)) // Masked Interrupt Status
#define UART1_ICR    (*(volatile uint32_t*)(UART1_BASE + 0x044)) // Interrupt Clear Register


void uart0_init(uint32_t baud_rate, const uint8_t uart_tx_pin, const uint8_t uart_rx_pin);
inline void uart0_putc(char c);

void uart0_enable_rx_interrupt(void);
void isr_uart0(void);


void uart1_init(uint32_t baud_rate, const uint8_t uart_tx_pin, const uint8_t uart_rx_pin);
inline void uart1_putc(char c);

void uart1_enable_rx_interrupt(void);
void isr_uart1(void);

#endif