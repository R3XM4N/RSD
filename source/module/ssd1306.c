#include "../../include/module/ssd1306.h"

#include "../../include/peripherals/rsd_i2c.h"
#include <stddef.h>

#define SSD1306_ADDR 0x3C

//reassurance until delay stops being cooked without scheduler
static void busy_wait(volatile uint32_t count){
    while (count--){
        __asm volatile("nop");
    }
}

uint8_t ssd1306_cmd(const uint8_t cmd){
    uint8_t buf[2] = {0x00, cmd}; /* 0x00 = control byte, "command follows" */
    return i2c_write_blocking(0, SSD1306_ADDR, buf, 2, 1);
}

uint8_t ssd1306_full_on_test(void){
    uint8_t ok = 1;
    ok &= ssd1306_cmd(0xAE);
    ok &= ssd1306_cmd(0xD5);
    ok &= ssd1306_cmd(0x80);
    ok &= ssd1306_cmd(0xA8);
    ok &= ssd1306_cmd(0x3F);
    ok &= ssd1306_cmd(0xD3);
    ok &= ssd1306_cmd(0x00);
    ok &= ssd1306_cmd(0x40);
    ok &= ssd1306_cmd(0x8D);
    ok &= ssd1306_cmd(0x14);
    ok &= ssd1306_cmd(0x20);
    ok &= ssd1306_cmd(0x00);
    ok &= ssd1306_cmd(0xA1);
    ok &= ssd1306_cmd(0xC8);
    ok &= ssd1306_cmd(0xDA);
    ok &= ssd1306_cmd(0x12);
    ok &= ssd1306_cmd(0x81);
    ok &= ssd1306_cmd(0xCF);
    ok &= ssd1306_cmd(0xD9);
    ok &= ssd1306_cmd(0xF1);
    ok &= ssd1306_cmd(0xDB);
    ok &= ssd1306_cmd(0x40);
    ok &= ssd1306_cmd(0xA5);
    busy_wait(3000000);         // needs to await in order to actually light up
    ok &= ssd1306_cmd(0xAF);    // display ON
    return ok;
}

uint8_t ssd1306_fill_white(void){
    uint8_t ok = 1;
    ok &= ssd1306_cmd(0xAE);
    ok &= ssd1306_cmd(0xD5); ok &= ssd1306_cmd(0x80);
    ok &= ssd1306_cmd(0xA8); ok &= ssd1306_cmd(0x3F);
    ok &= ssd1306_cmd(0xD3); ok &= ssd1306_cmd(0x00);
    ok &= ssd1306_cmd(0x40);
    ok &= ssd1306_cmd(0x8D); ok &= ssd1306_cmd(0x14);
    ok &= ssd1306_cmd(0x20); ok &= ssd1306_cmd(0x00);
    ok &= ssd1306_cmd(0xA1);
    ok &= ssd1306_cmd(0xC8);
    ok &= ssd1306_cmd(0xDA); ok &= ssd1306_cmd(0x12);
    ok &= ssd1306_cmd(0x81); ok &= ssd1306_cmd(0xCF);
    ok &= ssd1306_cmd(0xD9); ok &= ssd1306_cmd(0xF1);
    ok &= ssd1306_cmd(0xDB); ok &= ssd1306_cmd(0x40);
    ok &= ssd1306_cmd(0xA4);
    ok &= ssd1306_cmd(0xA6);
    busy_wait(3000000);         // needs to await in order to actually light up
    ok &= ssd1306_cmd(0xAF);    // display ON
    ok &= ssd1306_cmd(0x21); ok &= ssd1306_cmd(0); ok &= ssd1306_cmd(127); //col range
    ok &= ssd1306_cmd(0x22); ok &= ssd1306_cmd(0); ok &= ssd1306_cmd(7);   // page range

    static uint8_t buf[1 + 128 * 8];
    buf[0] = 0x40; // control byte stream
    for (size_t i = 1; i < sizeof(buf); i++){
        buf[i] = 0xFF;
    }
    ok &= i2c_write_blocking(0, SSD1306_ADDR, buf, sizeof(buf), 1);
    return ok;
}