#include "../../include/peripherals/rsd_i2c.h"

#include "../../include/peripherals/rsd_pin.h"
#include "../../include/peripherals/rsd_gpio.h"
#include "../../include/peripherals/rsd_reset.h"
#include "../../include/peripherals/rsd_raw_clock.h"
#include "../../include/interrupt/rsd_sys_tick.h"

#define I2C0_BASE               0x40090000u
#define I2C0_IC_CON             (*(volatile uint32_t*)(I2C0_BASE + 0x00)) // Control
#define I2C0_IC_TAR             (*(volatile uint32_t*)(I2C0_BASE + 0x04)) // Target
#define I2C0_IC_DATA_CMD        (*(volatile uint32_t*)(I2C0_BASE + 0x10)) // rx/tx data buffer/command
#define I2C0_IC_SS_SCL_HCNT     (*(volatile uint32_t*)(I2C0_BASE + 0x14)) // Standard s[eed scl high]
#define I2C0_IC_SS_SCL_LCNT     (*(volatile uint32_t*)(I2C0_BASE + 0x18)) // Standard s[eed scl low
#define I2C0_IC_CLR_TX_ABRT     (*(volatile uint32_t*)(I2C0_BASE + 0x54)) // Tx abort interrupt
#define I2C0_IC_ENABLE          (*(volatile uint32_t*)(I2C0_BASE + 0x6C)) // Enable reg
#define I2C0_IC_STATUS          (*(volatile uint32_t*)(I2C0_BASE + 0x70)) // stat
#define I2C0_IC_TX_ABRT_SOURCE  (*(volatile uint32_t*)(I2C0_BASE + 0x80)) // trasnmit abort

#define I2C1_BASE               0x40098000u
#define I2C1_IC_CON             (*(volatile uint32_t*)(I2C1_BASE + 0x00)) // Control
#define I2C1_IC_TAR             (*(volatile uint32_t*)(I2C1_BASE + 0x04)) // Target
#define I2C1_IC_DATA_CMD        (*(volatile uint32_t*)(I2C1_BASE + 0x10)) // rx/tx data buffer/command
#define I2C1_IC_SS_SCL_HCNT     (*(volatile uint32_t*)(I2C1_BASE + 0x14)) // Standard s[eed scl high]
#define I2C1_IC_SS_SCL_LCNT     (*(volatile uint32_t*)(I2C1_BASE + 0x18)) // Standard s[eed scl low
#define I2C1_IC_CLR_TX_ABRT     (*(volatile uint32_t*)(I2C1_BASE + 0x54)) // Tx abort interrupt
#define I2C1_IC_ENABLE          (*(volatile uint32_t*)(I2C1_BASE + 0x6C)) // Enable reg
#define I2C1_IC_STATUS          (*(volatile uint32_t*)(I2C1_BASE + 0x70)) // stat
#define I2C1_IC_TX_ABRT_SOURCE  (*(volatile uint32_t*)(I2C1_BASE + 0x80)) // trasnmit abort

#define GPIO_FUNC_I2C 3

void i2c0_init(const uint8_t sda_pin, const uint8_t scl_pin, const uint32_t baud_rate_hz){
    reset_await(26);
    reset_await(5);
    reset_await(6);
    gpio_pad_enable(sda_pin);
    gpio_pad_enable(scl_pin);
    volatile uint32_t *sda_pad = (volatile uint32_t*)PADS_GPIO(sda_pin);
    volatile uint32_t *scl_pad = (volatile uint32_t*)PADS_GPIO(scl_pin);
    *sda_pad |= (1u << 3); // PUE
    *scl_pad |= (1u << 3); // PUE
    (*(volatile uint32_t*)(IO_BANK0_BASE + sda_pin * 8 + 4)) = GPIO_FUNC_I2C;
    (*(volatile uint32_t*)(IO_BANK0_BASE + scl_pin * 8 + 4)) = GPIO_FUNC_I2C;
    I2C0_IC_ENABLE = 0;

    I2C0_IC_CON = (1u << 0)  // master mode
                | (1u << 1)  // speed = standard (01)
                | (1u << 5)  // restart enable
                | (1u << 6); // slave disabled

    uint32_t period = get_sys_clock_hz() / baud_rate_hz;
    I2C0_IC_SS_SCL_HCNT = (period / 2) - 3;
    I2C0_IC_SS_SCL_LCNT = (period / 2) - 1;
    I2C0_IC_ENABLE = 1;
}

void i2c0_write_blocking(const uint8_t target_7b, const uint8_t* data, const uint32_t data_byte_count, uint8_t send_stop){
    I2C0_IC_ENABLE = 0;
    I2C0_IC_TAR = target_7b;
    I2C0_IC_ENABLE = 1;

    for (uint32_t i = 0; i < data_byte_count; i++){
        while (!(I2C0_IC_STATUS & (1u << 1))){} // we waaaaaaaaait
        
        uint32_t cmd = data[i];
        if (send_stop && (i == data_byte_count - 1)){
            cmd |= (1u << 9); // stop last byte lmao
        }
        I2C0_IC_DATA_CMD = cmd;
    }
    while (I2C0_IC_STATUS & (1u << 0)){};// await clear bus activity
}