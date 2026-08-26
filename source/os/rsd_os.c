#include "../../include/os/rsd_os.h"

#include "../../include/peripherals/rsd_gpio.h"

void test_call(){
    gpio_enable_out(15);
    gpio_write(15);
}