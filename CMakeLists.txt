cmake_minimum_required(VERSION 3.13)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico2)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
project(rsd_pico2w C CXX ASM)
set(CMAKE_C_STANDARD 11)
pico_sdk_init()

add_executable(
    rsd_pico2w
    main.c
    asm_add.s
    source/peripherals/rsd_gpio.c
    # source/interrupt/rsd_sys_tick.c
    # source/peripherals/rsd_raw_clock.c
    source/peripherals/rsd_reset.c
    # source/peripherals/rsd_uart.c
    # source/interrupt/rsd_nvic.c
    # source/interrupt/rsd_interrupt.c
    # source/os/rsd_os.c
)

pico_add_extra_outputs(rsd_pico2w)
target_link_libraries(rsd_pico2w pico_runtime)

# pico_enable_stdio_usb(rsd_pico_one 1)
# pico_enable_stdio_uart(rsd_pico_one 0)
