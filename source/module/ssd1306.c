#include "../../include/module/ssd1306.h"

#include "../../include/peripherals/rsd_i2c.h"
#include "../../include/os/font/rsd_font_base.h"
#include "../../include/os/font/rsd_font_pixel5x7.h"

#include <stddef.h>
#include <string.h>

#define SSD1306_ADDR 0x3C

static uint8_t pixel_buffer[128 * 8];

typedef struct{
    uint8_t column_start;
    uint8_t column_end;
    uint8_t page_start;
    uint8_t page_end;
    uint16_t actions;
}cursor_data;

static cursor_data cursor_info = {0,0,0,0,0};
//reassurance until delay stops being cooked without scheduler
static void busy_wait(volatile uint32_t count){
    while (count--){
        __asm volatile("nop");
    }
}

//sends cmd prefix and ends afterwards
uint8_t ssd1306_cmd(const uint8_t cmd){
    uint8_t buf[2] = {0x00, cmd}; /* 0x00 = control byte, "command follows" */
    return i2c_write_blocking(0, SSD1306_ADDR, buf, 2, 1);
}

// sends one byte may not need end ass it doesn't work cuz i2c write blocking starts each time *yay*
// uint8_t ssd1306_send_raw_byte(const uint8_t raw, const uint8_t transfer_end){
//     uint8_t msg[1] = {raw};
//     return i2c_write_blocking(0, SSD1306_ADDR, msg , 1, transfer_end);
// }

// assumes stop after
uint8_t ssd1306_send_raw_data(const uint8_t* raw, const uint32_t length){
    return i2c_write_blocking(0, SSD1306_ADDR, raw, length, 1);
}

// sets cursor restrains
uint8_t ssd1306_curs_range(const uint8_t col_start, const uint8_t col_end, const uint8_t page_start, const uint8_t page_end){
    uint8_t instructions[7] = {0x00, 0x21, col_start, col_end,
                                    0x22, page_start, page_end};
    ssd1306_send_raw_data(instructions, 7);
}

//assumes transaction end and in bounds and full write area cursed ahh function
// uint8_t ssd1306_write_pixels(const uint8_t column, const uint8_t page, const uint16_t count){
    // if ((page * 128 + column + count) > 1024){
    //     return 0xFF;
    // }
    // // I am about to do perish this hurts my soul
    // uint8_t data[128 * 8 + 1] = {}; data[0] = 0x40;
    // for (size_t i = 0; i < count; i++){
    //     /* code */
    // }
    
    // ssd1306_send_raw_data
//     return -1;
// }

// rewrites whole gddr with local version
uint8_t ssd1306_write_whole(){
    static uint8_t data[8 * 128 + 1]; data[0] = 0x40;
    memcpy(&data[1], pixel_buffer, 1024);
    return ssd1306_send_raw_data(data, 1025);
}

void ssd1306_fill_zero(){
    for (uint16_t i = 0; i < 1024; i++){
        pixel_buffer[i] = 0x00;
    }
}

void ssd1306_fill_ones(){
    for (uint16_t i = 0; i < 1024; i++){
        pixel_buffer[i] = 0xFF;
    }
}

// Writes 0/1 to the specified bit position 64 height 128 column returns if bit changesd
uint8_t ssd1306_write_bit(const uint8_t row, const uint8_t column, const uint8_t value){
    if (row > 63 || column > 127){ return 0;}
    // page = row % 8 
    uint16_t index = (row / 8) * 128 + column;
    uint8_t changed_flag = value ^ (pixel_buffer[index] & (1u << row % 8));
    if (!changed_flag){
        return changed_flag;
    }
    if (value == 1){
        pixel_buffer[index] |= (1u << (row % 8));
    }
    else{
        pixel_buffer[index] &= ~(1u << (row % 8));
    }
    return changed_flag;
}

//writes a byte into a specific page and calumn position returns diff
uint8_t ssd1306_write_byte(const uint8_t page, const uint8_t column, const uint8_t byte){
    if (page > 7 || column > 127){ return 0x00;}
    uint8_t changes = byte ^ pixel_buffer[page * 128 + column];
    pixel_buffer[page * 128 + column] = byte;
    return changes;
}

static bit2pix_instance ssd1306_liner = {
    .write_space = {
        .c_cur_char = 0,
        .c_cur_line = 0,
        .m_char_p_line = 128 / 6,
        .m_line_count = 8

    },
    .write_buffer = pixel_buffer
};
//assuming adrewss space is whole screen
// void ssd1306_write_char(const char c, const uint8_t char_l_pos, const uint8_t line){

// }
//assuming adrewss space is whole screen and writes in current char pos
void ssd1306_write_char(const char c){
    if (ssd1306_liner.write_space.c_cur_char + 1 > ssd1306_liner.write_space.m_char_p_line){
        ssd1306_liner.write_space.c_cur_char = 0;
        ssd1306_liner.write_space.c_cur_line++;
        if (ssd1306_liner.write_space.c_cur_line + 1> ssd1306_liner.write_space.m_line_count){
            ssd1306_liner.write_space.c_cur_line = 0;
        }
    }
    for (uint8_t i = 0; i < 5; i++){
        ssd1306_write_byte(ssd1306_liner.write_space.c_cur_line, ssd1306_liner.write_space.c_cur_char * 6 + i, font5x7[(c - 32) * 5 + i]);
    }
    ssd1306_write_byte(ssd1306_liner.write_space.c_cur_line, ssd1306_liner.write_space.c_cur_char * 6 + 5, 0x00);
    ssd1306_liner.write_space.c_cur_char++;
}

// terminator flag is if it is terminade by nulls str
void ssd1306_write_str(const char* str, const uint16_t str_length, const uint8_t terminator_flag){
    for (uint16_t i = 0; i < str_length - terminator_flag; i++){
        ssd1306_write_char(str[i]);
    }
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
    ssd1306_fill_zero();
    // ssd1306_fill_ones();
    // {0x7F, 0x49, 0x49, 0x49, 0x36}
    ssd1306_write_bit(32, 64, 1);
    ssd1306_write_bit(0, 0, 1);
    ssd1306_write_bit(63, 127, 1);
    ssd1306_write_byte(4, 63, 0xFF);
    ssd1306_write_byte(4, 67, 0x7F);
    ssd1306_write_byte(4, 68, 0x49);
    ssd1306_write_byte(4, 69, 0x49);
    ssd1306_write_byte(4, 70, 0x49);
    ssd1306_write_byte(4, 71, 0x36);
    // ssd1306_write_char('H');
    // ssd1306_write_char('e');
    // ssd1306_write_char('l');
    // ssd1306_write_char('l');
    // ssd1306_write_char('.');
    
    ok &= ssd1306_write_whole();

    // static uint8_t buf[1 + 128 * 8];
    // buf[0] = 0x40; // control byte stream
    // for (size_t i = 1; i < sizeof(buf); i++){
    //     buf[i] = 0xFF;
    // }
    // ok &= i2c_write_blocking(0, SSD1306_ADDR, buf, sizeof(buf), 1);

    // uint8_t data_test[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    // ok &= ssd1306_write_pixels(data_test, 8);
    return ok;
}

uint8_t ssd1306_redraw(){
    return ssd1306_write_whole();
}