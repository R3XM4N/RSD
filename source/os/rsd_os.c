#include "../../include/os/rsd_os.h"

#include "../../include/peripherals/rsd_gpio.h"

static uint8_t int_led_on_flags = 0; //[0 enabled] [1 out?]

void test_call(){
    if (!(int_led_on_flags & (1u << 0))){
        gpio_enable_out(15);
        int_led_on_flags |= (1u << 0);
    }

    if (int_led_on_flags & (1u << 1)){
        gpio_clear(15);
        int_led_on_flags &= ~(1u << 1);
    }
    else{
        gpio_write(15);
        int_led_on_flags |= (1u << 1);
    }
}