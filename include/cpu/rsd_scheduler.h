#ifndef RSD_SCHEDULER_H
#define RSD_SCHEDULER_H

#include <stdint.h>

#define MAX_TASKS 4
#define TASK_STACK_WORD_SIZE 256

int8_t task_create(void (*entry)(void));
void task_pick_next(void);

void yield(void);
void start_scheduler(void);

void isr_pendsv(void);

#endif