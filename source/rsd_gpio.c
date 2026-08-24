#include "../include/rsd_gpio.h"

#include "../include/rsd_reset.h"
#include "../include/rsd_pin.h"
#include "../include/rsd_nvic.h"

void gpio_enable_out(const uint8_t gpio_pin){
    if ((gpio_pin < 23) || ((gpio_pin > 24) && (gpio_pin < 29))){
        reset_await(5);
        (*(volatile uint32_t*)(IO_BANK0_BASE + gpio_pin * 8 + 4)) = 5;
        GPIO_OE_SET = (1u << gpio_pin);
    }
}

void gpio_disable_out(const uint8_t gpio_pin){
    GPIO_OE_CLR = (1u << gpio_pin);
    (*(volatile uint32_t*)(IO_BANK0_BASE + gpio_pin * 8 + 4)) = 31;
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

#define PADS_BANK0_BASE  0x4001C000u

void private_gpio_pullup_en(const uint8_t gpio_pin){
    (*(volatile uint32_t*)(PADS_BANK0_BASE + 0x04 + ((gpio_pin) * 4)))  |= (1u << 3) | (1u << 6); // [3] pull up en, [6] in enable
}

void gpio_enable_fall_irq(const uint8_t gpio_pin){
    private_gpio_pullup_en(gpio_pin);

    uint8_t reg_idx = gpio_pin / 8;
    uint8_t bit_shift = (gpio_pin % 8) * 4;

    volatile uint32_t* inte_reg = (volatile uint32_t*)(IO_BANK0_BASE + 0x100 + (reg_idx * 4));

    *inte_reg |= (1u << (bit_shift + 2)); // [2] bit edge low for pin

    nvic_enable_irq(13);
}