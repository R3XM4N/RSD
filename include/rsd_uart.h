#ifndef RSD_UART_TICK_H
#define RSD_UART_TICK_H

#define UART0_BASE   0x40034000u
#define UART0_DR     (*(volatile uint32_t*)(UART0_BASE + 0x000)) // Data register
#define UART0_FR     (*(volatile uint32_t*)(UART0_BASE + 0x018)) // Flag register
#define UART0_IBRD   (*(volatile uint32_t*)(UART0_BASE + 0x024)) // Integer baud divisor
#define UART0_FBRD   (*(volatile uint32_t*)(UART0_BASE + 0x028)) // Fractional baud divisor
#define UART0_LCR_H  (*(volatile uint32_t*)(UART0_BASE + 0x02C)) // Line control
#define UART0_CR     (*(volatile uint32_t*)(UART0_BASE + 0x030)) // Control register
 

#endif