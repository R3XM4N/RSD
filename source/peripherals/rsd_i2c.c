#include "../../include/peripherals/rsd_i2c.h"

#include <stddef.h>

#include "../../include/peripherals/rsd_pin.h"
#include "../../include/peripherals/rsd_gpio.h"
#include "../../include/peripherals/rsd_reset.h"
#include "../../include/peripherals/rsd_raw_clock.h"

static uint32_t i2c_base(const i2c_instance_t instance){
    return (instance == I2C_INSTANCE_0) ? 0x40090000u : 0x40098000u;
}
static uint32_t i2c_reset_bit(const i2c_instance_t instance){
    return (instance == I2C_INSTANCE_0) ? 4u : 5u;
}

// IC not I2C to make crossreferencing via datasheet sensible
#define OFF_IC_CON                      0x00u
#define OFF_IC_TAR                      0x04u
#define OFF_IC_DATA_CMD                 0x10u
#define OFF_IC_SS_SCL_HCNT              0x14u // Standard mode scl high period
#define OFF_IC_SS_SCL_LCNT              0x18u // Standard mode scl low period
#define OFF_IC_FS_SCL_HCNT              0x1Cu // fast-mode scl high period
#define OFF_IC_FS_SCL_LCNT              0x20u // fast-mode scl low period
#define OFF_IC_RAW_INTR_STAT            0x34u // interrupt flags
#define OFF_IC_RX_TL                    0x38u // rx fifo treashold
#define OFF_IC_TX_TL                    0x3Cu // tx fifo treshold
#define OFF_IC_CLR_TX_ABRT              0x54u // read results in clear of tx_abrt flag
#define OFF_IC_ENABLE                   0x6Cu
#define OFF_IC_STATUS                   0x70u
#define OFF_IC_ENABLE_STATUS            0x9Cu

#define IC_STATUS_TFNF_BITS             (1u << 1) // trasmit fifo not full bit
#define IC_STATUS_TFE_BITS              (1u << 2) // transmit fifo empty? bit
#define IC_STATUS_MST_ACTIVITY_BITS     (1u << 5) // state of the i2c master (0-idle 1-active)
#define IC_DATA_CMD_STOP_BIT            (1u << 9) // end transfer bit flag
#define IC_RAW_INTR_STAT_TX_ABRT_BITS   (1u << 6) // 1 if transport aborted (nack etc)

#define IC_STATUS_RFNE_BITS             (1u << 3) // receive FIFO not empty
#define IC_DATA_CMD_READ_BIT            (1u << 8)

static uint8_t current_target[2] = {0xFF, 0xFF}; // target for each i2c lane {i2c0_tar, i2c1_tar}

/// @brief Safe repoint of target on chosen i2c instance because device must be fully disabled beforehand
/// @param instance the desired instance of i2c
/// @param target_7b_addr target device adresss
static void i2c_set_target(const i2c_instance_t instance, const uint8_t target_7b_addr){
    if (target_7b_addr == current_target[instance]){
        return;
    }

    uint32_t desired_i2c = i2c_base(instance);
    
    REG(desired_i2c, OFF_IC_ENABLE) = 0;
    while (REG(desired_i2c, OFF_IC_ENABLE_STATUS) & 1u){ } // await true disable 

    REG(desired_i2c, OFF_IC_TAR) = target_7b_addr;
    REG(desired_i2c, OFF_IC_ENABLE) = 1;
    while (!(REG(desired_i2c, OFF_IC_ENABLE_STATUS) & 1u)){  } // await true enable

    current_target[instance] = target_7b_addr;
}

void i2c_init(const i2c_instance_t instance, const uint8_t sda_pin, const uint8_t scl_pin, const uint32_t baud_rate_hz){
    uint32_t desired_i2c = i2c_base(instance); //base regsiter of the desired i2c isntance
    reset_await(6);
    reset_await(9);
    reset_await(i2c_reset_bit(instance)); // the only i2c sole await
    gpio_pad_enable(sda_pin);
    gpio_pad_enable(scl_pin);

    volatile uint32_t* sda_pad = (volatile uint32_t*)PADS_GPIO(sda_pin);
    volatile uint32_t* scl_pad = (volatile uint32_t*)PADS_GPIO(scl_pin);
    *sda_pad |= (1u << 3); // internal pull-up en
    *scl_pad |= (1u << 3);
    gpio_set_funcsel(sda_pin, I2C_FUNCSEL);
    gpio_set_funcsel(scl_pin, I2C_FUNCSEL);

    REG(desired_i2c, OFF_IC_ENABLE) = 0;
    // REG(desired_i2c, OFF_IC_CON) =
    //     (1u << 0) | // master
    //     (1u << 1) | // single just 100khz speed for now
    //     (1u << 5) | // restart en
    //     (1u << 6) | // slave disable
    //     (1u << 8);  // tc empty cntrl

    uint32_t con = (1u<<0) | (1u<<5) | (1u<<6) | (1u<<8); // master, restart, slave_dis, tx_empty_ctrl
    uint32_t period = get_sys_clock_hz() / baud_rate_hz;
    if (baud_rate_hz <= 100000){
        con |= (1u << 1); // SPEED = standard
        REG(desired_i2c, OFF_IC_SS_SCL_HCNT) = (period * 2u) / 5u;
        REG(desired_i2c, OFF_IC_SS_SCL_LCNT) = period - REG(desired_i2c, OFF_IC_SS_SCL_HCNT);
    }
    else if (baud_rate_hz <= 1000000){
        con |= (2u << 1); // SPEED = fast (covers both 400kHz fast-mode and up to 1MHz fast-mode-plus)
        REG(desired_i2c, OFF_IC_FS_SCL_HCNT) = (period * 2u) / 5u;
        REG(desired_i2c, OFF_IC_FS_SCL_LCNT) = period - REG(desired_i2c, OFF_IC_FS_SCL_HCNT);
    }
    else{return;} // No major error reporting yet

    REG(desired_i2c, OFF_IC_CON) = con;
    REG(desired_i2c, OFF_IC_RX_TL) = 0;
    REG(desired_i2c, OFF_IC_TX_TL) = 0;
    current_target[instance] = 0xFF; // force first write to set the target

    REG(desired_i2c, OFF_IC_ENABLE) = 1;
    while (!(REG(desired_i2c, OFF_IC_ENABLE_STATUS) & 1u)){  } // await true enable
}

uint8_t i2c_write_blocking(const i2c_instance_t inst, const uint8_t target_7b, const uint8_t* data, const uint32_t data_byte_count, const uint8_t send_stop){
    uint32_t i2c_c_base = i2c_base(inst);
    i2c_set_target(inst, target_7b);

    for (uint32_t i = 0; i < data_byte_count; i++){
        while (!(REG(i2c_c_base, OFF_IC_STATUS) & IC_STATUS_TFNF_BITS)){  } // await room in fifo
        uint32_t cmd = data[i];
        if (send_stop && (i == data_byte_count - 1)){
            cmd |= IC_DATA_CMD_STOP_BIT;
        }
        REG(i2c_c_base, OFF_IC_DATA_CMD) = cmd;
    }

    while (!(REG(i2c_c_base, OFF_IC_STATUS) & IC_STATUS_TFE_BITS) || (REG(i2c_c_base, OFF_IC_STATUS) & IC_STATUS_MST_ACTIVITY_BITS)){ } // Wait for fifo none and idle aka full finish
    if (REG(i2c_c_base, OFF_IC_RAW_INTR_STAT) & IC_RAW_INTR_STAT_TX_ABRT_BITS){
        (void)REG(i2c_c_base, OFF_IC_CLR_TX_ABRT); //clear abort flag
        return 0;
    }
    return 1;
}

uint8_t i2c_addr_sweep(const i2c_instance_t instance){
    for (uint8_t addr = 0x08; addr < 0x78; addr++){
        uint8_t probe = 0x00;
        if (i2c_write_blocking(instance, addr, &probe, 1, 1)) {
            return addr;
        }
    }
    return 0xFF; // NONE FOUND invalid addrs returns
}

uint8_t i2c_get_active(){
    uint8_t states = 0;
    if (0) { states |= (1u << 0);} // state of instance 0
    if (0) { states |= (1u << 1);} // state of instance 1    
    return states;
}

uint8_t i2c_read_blocking(const i2c_instance_t instance, const uint8_t target_7b, uint8_t* out, const uint32_t len){
    uint32_t base = i2c_base(instance);
    i2c_set_target(instance, target_7b);

    for (uint32_t i = 0; i < len; i++){
        while (!(REG(base, OFF_IC_STATUS) & IC_STATUS_TFNF_BITS)){ } // room to queue requests

        uint32_t cmd = IC_DATA_CMD_READ_BIT;
        if (i == len - 1) cmd |= IC_DATA_CMD_STOP_BIT;
        REG(base, OFF_IC_DATA_CMD) = cmd; // request a byte
    }

    for (uint32_t i = 0; i < len; i++){
        while (!(REG(base, OFF_IC_STATUS) & IC_STATUS_RFNE_BITS)){ } // await data
        out[i] = (uint8_t)REG(base, OFF_IC_DATA_CMD); // reading this register pulls from RX FIFO
    }

    if (REG(base, OFF_IC_RAW_INTR_STAT) & IC_RAW_INTR_STAT_TX_ABRT_BITS){
        (void)REG(base, OFF_IC_CLR_TX_ABRT);
        return 0;
    }
    return 1;
}