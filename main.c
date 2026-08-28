#include <stdint.h>

#include "include/interrupt/rsd_sys_tick.h"
#include "include/peripherals/rsd_gpio.h"
#include "include/peripherals/rsd_raw_clock.h"
#include "include/interrupt/rsd_interrupt.h"
#include "include/cpu/rsd_scheduler.h"

#include "include/os/rsd_os.h"
int32_t asm_add(int32_t a, int32_t b);

void taskA(void){
    while (1){
        gpio_write(16);
        delay_ms(200);
        gpio_clear(16);
        delay_ms(200);
        // yield();
    }
}

void taskB(void){
    while (1){
        gpio_write(17);
        delay_ms(500);
        gpio_clear(17);
        delay_ms(500);
        // yield();
    }
}

void taskC(void){
    while (1){
        gpio_write(13);
        delay_ms(350);
        gpio_clear(13);
        delay_ms(350);
        // yield();
    }
}

int main(void){
    sys_tick_init(get_sys_clock_hz());
    irq_init();
    irq_register(IO_IRQ_BANK0_NUM, isr_io_bank0);
    gpio_enable_out(16);
    gpio_enable_out(17);
    gpio_enable_out(13);

    gpio_enable_fall_irq(14);
    
    task_create(taskA);
    task_create(taskB);
    task_create(taskC);
    start_scheduler();

    // if (asm_add(5, 5) > 9){
    //     gpio_write(16);
    // }
    // gpio_write(17);

    // test_call();
    while (1){
        // gpio_write(17);
        // delay_ms(500);
        // gpio_clear(17);
        // delay_ms(500);
    }
}