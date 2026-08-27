#include "../../include/cpu/rsd_scheduler.h"

uint32_t* task_sp[MAX_TASKS];
uint8_t active_task = 0;
uint8_t num_of_tasks = 0;

static uint32_t task_stacks[MAX_TASKS][TASK_STACK_WORD_SIZE]; // Whole data of tasks pre-regsitered

void task_create(void (*entry)(void)){
    uint32_t* sp = &task_stacks[num_of_tasks][TASK_STACK_WORD_SIZE];

    sp -= 9;
    for (uint8_t i = 0; i < 8; i++){
        sp[i] = 0;
    }
    sp[8] = (uint32_t)entry;

    task_sp[num_of_tasks] = sp;
    num_of_tasks++;
}

void task_pick_next(void){
    active_task = (active_task + 1) % num_of_tasks;
}