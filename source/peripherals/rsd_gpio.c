#include "../../include/peripherals/rsd_gpio.h"

#include "../../include/peripherals/rsd_reset.h"
#include "../../include/peripherals/rsd_pin.h"
#include "../../include/interrupt/rsd_interrupt.h"

#define PADS_BANK0_BASE      0x40038000u
#define PADS_OFFSET(n)       (0x04u + (n) * 4u)
#define PADS_GPIO(n)         (PADS_BANK0_BASE + PADS_OFFSET(n))
#define PADS_GPIO_CLR(n)     (PADS_BANK0_BASE + 0x3000u + PADS_OFFSET(n)) // atomic clear alias
#define PADS_GPIO_SET(n)     (PADS_BANK0_BASE + 0x2000u + PADS_OFFSET(n)) // atomic set alias

#define PADS_ISO_BIT (1u << 8)
#define PADS_OD_BIT  (1u << 7)
#define PADS_IE_BIT  (1u << 6)

static void gpio_pad_enable(const uint8_t gpio_pin){
    reset_await(9); // PADS_BANK0 out of reset
    (*(volatile uint32_t*)PADS_GPIO_CLR(gpio_pin)) = PADS_ISO_BIT | PADS_OD_BIT;
    (*(volatile uint32_t*)PADS_GPIO_SET(gpio_pin)) = PADS_IE_BIT;
}

void gpio_enable_out(const uint8_t gpio_pin){
    if ((gpio_pin < 23) || ((gpio_pin > 24) && (gpio_pin < 29))){
        reset_await(6); // IO_BANK0 out of reset that has moved from 5 to 6 on pico 2040->2350
        gpio_pad_enable(gpio_pin);
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

void private_gpio_pullup_en(const uint8_t gpio_pin){
    gpio_pad_enable(gpio_pin); // clears ISO, sets IE
    volatile uint32_t* pad_register = (volatile uint32_t*)PADS_GPIO(gpio_pin);
    *pad_register = (*pad_register & ~(1u << 2)) | (1u << 3) | (1u << 1); // clear pde[2], set pue[3], schmitt[1] no ie[6]
}

void gpio_enable_fall_irq(const uint8_t gpio_pin){
    reset_await(6);
    (*(volatile uint32_t*)(IO_BANK0_BASE + (gpio_pin * 8) + 4)) = 5; //sio func 5
    private_gpio_pullup_en(gpio_pin);
    GPIO_OE_CLR = (1u << gpio_pin);

    uint8_t reg_idx = gpio_pin / 8;
    uint8_t bit_shift = (gpio_pin % 8) * 4;

    volatile uint32_t* inte_reg = (volatile uint32_t*)(IO_BANK0_BASE + 0x248 + (reg_idx * 4));
    *inte_reg |= (1u << (bit_shift + 2));

    nvic_enable_irq(IO_IRQ_BANK0_NUM); 
}