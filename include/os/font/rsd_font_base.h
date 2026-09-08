#ifndef RSD_FONT_BASE_H
#define RSD_FONT_BASE_H

#include <stdint.h>

typedef struct{
    const uint8_t m_char_p_line;    // max ammount of characters fitting one line
    const uint8_t m_line_count;     // max ammount of lines in the space
    uint8_t c_cur_char;             // current cursor character ina line
    uint8_t c_cur_line;             // current line of the cursor
}type_space;

typedef struct{
    const uint8_t f_p_height;   // font pixel height
    const uint8_t f_p_width;    // font pixel width
    const uint8_t** f_arr;      // pointer to the fonts array definition
    const uint8_t f_arr_max;    // font definition array max index
}bit_font;

typedef struct{
    type_space write_space; //space definitions
    uint8_t* write_buffer;  //the space to write into
    bit_font font;
}bit2pix_instance;

// void type_char(const bit2pix_instance write_space, const char ch);

#endif