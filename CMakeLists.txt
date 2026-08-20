cmake_minimum_required(VERSION 3.13)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
project(rsd_pico_one C CXX ASM)
set(CMAKE_C_STANDARD 11)
pico_sdk_init()

add_executable(rsd_pico_one main.c asm_add.s)
pico_add_extra_outputs(rsd_pico_one)
target_link_libraries(rsd_pico_one pico_stdlib)

pico_enable_stdio_usb(rsd_pico_one 1)
pico_enable_stdio_uart(rsd_pico_one 0)
