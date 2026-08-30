#include "../../include/module/ssd1306.h"

#include "../../include/peripherals/rsd_i2c.h"

void ssd1306_cmd(const uint8_t cmd){
    uint8_t buf[2] = {0x00, cmd};
    i2c0_write_blocking(0x3C, buf, 2, 1);
}

void ssd1306_full_on_test(void){
    ssd1306_cmd(0xAE);             // display off
    ssd1306_cmd(0xD5); ssd1306_cmd(0x80); // clock divide
    ssd1306_cmd(0xA8); ssd1306_cmd(0x3F); // multiplex ratio (64-1)
    ssd1306_cmd(0xD3); ssd1306_cmd(0x00); // display offset
    ssd1306_cmd(0x40);             // start line = 0
    ssd1306_cmd(0x8D); ssd1306_cmd(0x14); // charge pump ON -- critical, easy to miss
    ssd1306_cmd(0x20); ssd1306_cmd(0x00); // addressing mode
    ssd1306_cmd(0xA1);             // segment remap
    ssd1306_cmd(0xC8);             // COM scan direction
    ssd1306_cmd(0xDA); ssd1306_cmd(0x12); // COM pins config
    ssd1306_cmd(0x81); ssd1306_cmd(0xCF); // contrast
    ssd1306_cmd(0xD9); ssd1306_cmd(0xF1); // precharge period
    ssd1306_cmd(0xDB); ssd1306_cmd(0x40); // VCOMH deselect level
    ssd1306_cmd(0xA4);
    ssd1306_cmd(0xA6);
    ssd1306_cmd(0xAF);
}