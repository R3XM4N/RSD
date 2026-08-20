#include <stdint.h>

#include "def_gpio.h"

int32_t asm_add(int32_t a, int32_t b);

static void scuff_sleep(volatile uint32_t count){
    while (count--){
        __asm__("nop");
    }
}

void gpio_enable(const uint8_t gpio_pin){
    if ((gpio_pin < 23) || ((gpio_pin > 24) && (gpio_pin < 29))){
        RESETS_RESET &= ~(1u << 5);
        while (!(RESETS_DONE & (1u << 5))){}
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

int main(void){
    gpio_enable(25);
    gpio_enable(16);
    gpio_write(16);    

    while (1){
        int32_t result = asm_add(3, 4);
        if (result > 5){
            gpio_write(25);
        }
        scuff_sleep(8000000);
        result = asm_add(3, 1);
        if (result < 5){
            gpio_clear(25);
        }
        scuff_sleep(8000000);
    }
}