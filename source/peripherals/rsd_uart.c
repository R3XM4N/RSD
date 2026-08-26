// #include "../../include/peripherals/rsd_uart.h"

// #include "../../include/peripherals/rsd_pin.h"
// #include "../../include/peripherals/rsd_reset.h"
// #include "../../include/peripherals/rsd_raw_clock.h"
// #include "../../include/interrupt/rsd_nvic.h"

// #define RSD_IRQ_UART0 20
// #define RSD_IRQ_UART1 21

// typedef struct{
//     uint8_t tx_pin;
//     uint8_t rx_pin;
//     uint8_t enabled_flag;
// } uart_conf;

// uart_conf UART0_CONF = {255, 255, 0};
// uart_conf UART1_CONF = {255, 255, 0};

// /// @param uart_id 0 or 1
// void private_uart_init(uint8_t uart_id, uint32_t baud_rate, const uint8_t uart_tx_pin, const uint8_t uart_rx_pin){
//     if (uart_id > 1){ return;}
    
//     // Eyo hopefully it ain't a switchup of rx and tx
//     (*(volatile uint32_t*)(IO_BANK0_BASE + uart_tx_pin * 8 + 4)) = 2;
//     (*(volatile uint32_t*)(IO_BANK0_BASE + uart_rx_pin * 8 + 4)) = 2;
    
//     uint32_t clock_rate = get_sys_clock_hz();
//     uint32_t divisor_x64 = (clock_rate * 4) / baud_rate;
    
//     if (uart_id == 0){
//         UART0_IBRD = divisor_x64 >> 6;
//         UART0_FBRD = divisor_x64 & 0x3F;

//         UART0_LCR_H = (0x3u << 5 | 1u << 4);
//         UART0_CR = (1u << 0) | (1u << 8) | (1u << 9);
//     }
//     else if(uart_id == 1){
//         UART1_IBRD = divisor_x64 >> 6;
//         UART1_FBRD = divisor_x64 & 0x3F;

//         UART1_LCR_H = (0x3u << 5 | 1u << 4);
//         UART1_CR = (1u << 0) | (1u << 8) | (1u << 9);
//     }
// }

// void uart0_init(uint32_t baud_rate, const uint8_t uart_tx_pin, const uint8_t uart_rx_pin){
//     if (UART0_CONF.enabled_flag != 0){ return;}
//     if (!((uart_tx_pin == 0) || (uart_tx_pin == 12) || (uart_tx_pin == 16))){ return;}
//     if (!((uart_rx_pin == 1) || (uart_rx_pin == 13) || (uart_rx_pin == 17))){ return;}
//     reset_await(22);

//     private_uart_init(0, baud_rate, uart_tx_pin, uart_rx_pin);
//     UART0_CONF.enabled_flag = 1;
// }

// void uart0_putc(char c){
//     while (UART0_FR & (1u << 5)){} // wait if no free space
//     UART0_DR = (uint32_t)c;
// }

// void uart1_init(uint32_t baud_rate, const uint8_t uart_tx_pin, const uint8_t uart_rx_pin){
//     if (UART1_CONF.enabled_flag != 0){ return;}
//     if (!((uart_tx_pin == 4) || (uart_tx_pin == 8) || (uart_tx_pin == 20))){ return;}
//     if (!((uart_rx_pin == 5) || (uart_rx_pin == 9) || (uart_rx_pin == 21))){ return;}
//     reset_await(23);

//     private_uart_init(1, baud_rate, uart_tx_pin, uart_rx_pin);
//     UART1_CONF.enabled_flag = 1;
// }

// void uart1_putc(char c){
//     while (UART1_FR & (1u << 5)){} // wait if no free space
//     UART1_DR = (uint32_t)c;
// }

// void uart0_enable_rx_interrupt(void){
//     UART0_IMSC |= (1u << 4) | (1u << 6); // interrupt on rx fifo data [4] and timeout on no data [6] 
//     nvic_enable_irq(RSD_IRQ_UART0);
// }

// void isr_uart0(void){
//     while (!(UART0_FR & (1u << 4))){
//         char recieved_char = (char)(UART0_DR & 0xFF);

//         // CODE GOES HERE LATER
//     }
//     UART0_ICR = (1u << 4) | (1u << 6);
// }

// void uart1_enable_rx_interrupt(void){
//     UART1_IMSC |= (1u << 4) | (1u << 6); // interrupt on rx fifo data [4] and timeout on no data [6] 
//     nvic_enable_irq(RSD_IRQ_UART1);
// }

// void isr_uart1(void){
//     while (!(UART1_FR & (1u << 4))){
//         char recieved_char = (char)(UART1_DR & 0xFF);

//         // CODE GOES HERE LATER MAYBE xd
//     }
//     UART1_ICR = (1u << 4) | (1u << 6);
// }

