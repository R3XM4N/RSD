#include "../include/rsd_gpio.h"

#include "../include/rsd_reset.h"
#include "../include/rsd_pin.h"

void gpio_enable(const uint8_t gpio_pin){
    if ((gpio_pin < 23) || ((gpio_pin > 24) && (gpio_pin < 29))){
        reset_await(5);
        (*(volatile uint32_t*)(IO_BANK0_BASE + gpio_pin * 8 + 4)) = 5;
        GPIO_OE_SET = (1u << gpio_pin);
    }
}

void gpio_write(const uint8_t gpio_pin){
    if ((gpio_pin < 23) || ((gpio_pin > 24) && (gpio_pin < 29))){
            GPIO_OUT_SET = (1u << gpio_pin);
    }
}

void gpio_clear(const uint8_t gpio_pin){
    if ((gpio_pin < 23) || ((gpio_pin > 24) && (gpio_pin < 29))){
        GPIO_OUT_CLR = (1u << gpio_pin);
    }
}