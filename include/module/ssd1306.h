#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>

// ret 1 if ack 0 if abrt also sends a cmd
uint8_t ssd1306_cmd(const uint8_t cmd);

// inits and force pixels on via 0xa5 cmd override
uint8_t ssd1306_full_on_test(void);

// inits and fills every pixel white/full
uint8_t ssd1306_fill_white(void);

//test
void ssd1306_write_str(const char* str, const uint16_t str_length, const uint8_t terminator_flag);

uint8_t ssd1306_redraw();
#endif