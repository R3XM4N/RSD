#include <stdint.h>

// #include "include/interrupt/rsd_sys_tick.h"
#include "include/peripherals/rsd_gpio.h"
// #include "include/peripherals/rsd_raw_clock.h"
// #include "include/interrupt/rsd_interrupt.h"

// #include "include/os/rsd_os.h"
int32_t asm_add(int32_t a, int32_t b);


int main(void){
    // sys_tick_init(get_sys_clock_hz());
    // irq_init();
    // irq_register(13, isr_io_bank0);

    // (*(volatile uint32_t*)0xE000E200u) = (1u << 13);

    gpio_enable_out(16);
    if (asm_add(5, 5) > 9){
        gpio_write(16);
    }
    if (asm_add(5, 5) < 15){
        gpio_clear(16);
    }
    // gpio_clear(16);
    
    // gpio_enable_fall_irq(15);

    // test_call();
    while (1){
        // int32_t result = asm_add(3, 4);
        // if (result > 5){
        //     gpio_write(25);
        // }
        // delay_ms(500);
        // gpio_write(16);

        // result = asm_add(3, 1);
        // if (result < 5){
        //     gpio_clear(25);
        // }
        // // if (!gpio_read(18)){
        // //     gpio_write(20);
        // // }
        // // else {
        // //     gpio_clear(20);
        // // }
        
        // delay_ms(500);
    }
}