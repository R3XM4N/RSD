#include <stdint.h>

#include "include/rsd_sys_tick.h"
#include "include/rsd_gpio.h"
#include "include/rsd_raw_clock.h"

int32_t asm_add(int32_t a, int32_t b);

int main(void){
    sys_tick_init(get_sys_clock_hz());
    gpio_enable_out(25);
    gpio_enable_out(16);

    while (1){
        int32_t result = asm_add(3, 4);
        if (result > 5){
            gpio_write(25);
        }
        delay_ms(500);
        gpio_write(16);

        result = asm_add(3, 1);
        if (result < 5){
            gpio_clear(25);
        }
        delay_ms(500);
    }
}