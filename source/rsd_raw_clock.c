#include "../include/rsd_raw_clock.h"

#define XOSC_FREQ_HZ 12000000u

static uint32_t pll_sys_get_hz(void){
    uint32_t refdiv   = PLL_SYS_CS & 0x3Fu;
    uint32_t fbdiv    = PLL_SYS_FBDIV_INT & 0xFFFu;
    uint32_t postdiv1 = (PLL_SYS_PRIM >> 16) & 0x7u;
    uint32_t postdiv2 = (PLL_SYS_PRIM >> 12) & 0x7u;

    uint32_t vco_freq = (XOSC_FREQ_HZ / refdiv) * fbdiv;
    return vco_freq/(postdiv1 * postdiv2);
}

uint32_t clocks_get_sys_hz(void){
    return pll_sys_get_hz();
}