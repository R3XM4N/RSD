#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>

void ssd1306_cmd(const uint8_t cmd);
void ssd1306_test_init(void);
void ssd1306_full_on_test(void);

#endif