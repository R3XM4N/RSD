#include "../../include/peripherals/rsd_pin.h"

void gpio_set_funcsel(const uint8_t pin, const funcsel funcsel_id){
    if (pin > 29){
        return;
    }
    // TO DO: Correct pin to correct funcsel 
    REG(IO_BANK0_BASE, 8u * pin + 4u) = funcsel_id;
}