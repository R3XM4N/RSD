### This is a repo in progress. It is a project meant for the Pico 2 W
Current main purpouse is to serve as a remote pullable and commitable test repo and will slowly transition into what it is supposed to be, but i am not a magician and need to learn, cuz i am a human creature.
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