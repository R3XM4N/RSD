#include "../../include/os/rsd_os.h"

#include "../../include/rsd_gpio.h"

void test_call(){
    gpio_enable_out(20);
    gpio_write(20);
}