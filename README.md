# (WIP) RSD - Really Simple Device
A simple os-like runtime/system. Intended as multipurpouse hobby project for the Pico 2W that is inteded to be able to do many things currently only working on backand. It is intendeed with certain PGIO connections certain places i2c etc.
# Current state 
🟢 Basic round robin scheduler  
🟢 Basic GPI0  
🔵 Basic UART  
🟢 Basic interaction interfacing (4 Out leds + 2 interruptable buttons) ⚫   
🟢 Basic SYS_LED/ERROR_LED state support  ⚫  
🟠 Basic I2C  
🟠 Implementing "os" layer awarness of pin states  
🟠 Getting a simple driver for a eternal display module

🟣 More module drivers  
🟣 Kernel - program seperation  
🟣 Actual apps (simple calc, HID macro runner/device, func test "game")  

⚫ - HW - Hardware and sw not just code

## File formating:
The repo follows these file content rules 
### Headers:
```c
#ifndef RSD_NAME_H
#define RSD_NAME_H

#include <system includes>

#include "local includes"

void code();

#endif
```
### Source files
```c
#include "include/location/rsd_name.h"

#include <system includes>

#include "local includes"

void code(){
    implementation();
}
```
Last note: i am not a magician and need to learn, cuz i am a human creature.