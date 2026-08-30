#include <stdint.h>

#include "include/interrupt/rsd_sys_tick.h"
#include "include/peripherals/rsd_gpio.h"
#include "include/peripherals/rsd_raw_clock.h"
#include "include/peripherals/rsd_i2c.h"
#include "include/interrupt/rsd_interrupt.h"
#include "include/cpu/rsd_scheduler.h"
#include "include/os/rsd_os.h"
#include "include/os/rsd_system.h"

int32_t asm_add(int32_t a, int32_t b);
volatile static uint8_t running = 0;

void taskA(void){
    while (1){
        gpio_write(SYS_MSG_LED_0);
        delay_ms(200);
        gpio_clear(SYS_MSG_LED_0);
        delay_ms(200);
        // yield();
    }
}

void taskB(void){
    while (1){
        gpio_write(SYS_MSG_LED_1);
        delay_ms(500);
        gpio_clear(SYS_MSG_LED_1);
        delay_ms(500);
        // yield();
    }
}

void taskC(void){
    while (1){
        gpio_write(SYS_MSG_LED_2);
        delay_ms(350);
        gpio_clear(SYS_MSG_LED_2);
        delay_ms(350);
        // yield();
    }
}

void ini_sys_base(){
    gpio_enable_out(SYS_LED);
    gpio_enable_out(SYS_ERROR_LED);
    gpio_enable_out(SYS_MSG_LED_0);
    gpio_enable_out(SYS_MSG_LED_1);
    gpio_enable_out(SYS_MSG_LED_2);
    gpio_enable_out(SYS_MSG_LED_3);
    gpio_write(SYS_LED);
    gpio_write(SYS_ERROR_LED);

    sys_tick_init(get_sys_clock_hz());
    irq_init();
    irq_register(IO_IRQ_BANK0_NUM, isr_io_bank0);

    gpio_enable_fall_irq(SYS_MSG_CTRL_BTN_0);
    gpio_enable_fall_irq(SYS_MSG_CTRL_BTN_1);

    gpio_clear(SYS_ERROR_LED);
}

int main(void){
    ini_sys_base();
    
    task_create(taskA);
    task_create(taskB);
    task_create(taskC);
    start_scheduler();
    

    while (1){
        // gpio_write(17);
        // delay_ms(500);
        // gpio_clear(17);
        // delay_ms(500);
    }
}