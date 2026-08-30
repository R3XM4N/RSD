#include "../../include/os/rsd_os.h"

#include "../../include/peripherals/rsd_gpio.h"
#include "../../include/os/rsd_system.h"

static uint8_t int_led_on_flags = 0;

void test_call(){
    if (int_led_on_flags == 1){
        gpio_clear(SYS_ERROR_LED);
        int_led_on_flags = 0;
    }
    else{
        gpio_write(SYS_ERROR_LED);
        int_led_on_flags = 1;
    }
}