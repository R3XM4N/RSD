#include "../include/rsd_reset.h"

void reset_await(uint8_t bit_pos){
    RESETS_RESET &= ~(1u << bit_pos);
    while (!(RESETS_DONE & (1u << bit_pos))){}
}
