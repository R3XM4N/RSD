#include "../../include/cpu/rsd_scheduler.h"

uint32_t* task_sp[MAX_TASKS];
uint8_t active_task = 0;
uint8_t num_of_tasks = 0;
uint8_t scheduler_running = 0;

static uint32_t task_stacks[MAX_TASKS][TASK_STACK_WORD_SIZE]; // Whole data of tasks pre-regsitered

#define EXC_RETURN_THREAD_MSP 0xFFFFFFF9u

void task_create(void (*entry)(void)){
    uint32_t* sp = &task_stacks[num_of_tasks][TASK_STACK_WORD_SIZE];

    sp -= 8;
    sp[0] = 0;
    sp[1] = 0;
    sp[2] = 0;
    sp[3] = 0;
    sp[4] = 0;
    sp[5] = 0;
    sp[6] = (uint32_t)entry;  // pc where exception return "resumes"
    sp[7] = 0x01000000u;      // xPSR -- Thumb bit (24) MUST be set or hardfault

    sp -= 9;
    for (uint8_t i = 0; i < 8; i++){
        sp[i] = 0;
    }
    sp[8] = EXC_RETURN_THREAD_MSP;

    task_sp[num_of_tasks] = sp;
    num_of_tasks++;
}

void task_pick_next(void){
    active_task = (active_task + 1) % num_of_tasks;
}

#define SCB_ICSR       (*(volatile uint32_t*)(0xE000ED04u))
#define SCB_SHPR3      (*(volatile uint32_t*)(0xE000ED20u))
#define PENDSVSET_BIT  (1u << 28)

void yield(void){
    SCB_ICSR = PENDSVSET_BIT;
    __asm volatile ("dsb");
    __asm volatile ("isb");
}

void start_scheduler(void){
    SCB_SHPR3 |= (0xFFu << 16); // pendsv = low prio isr
    yield();
    while (1){ __asm volatile ("wfi");} // waste should not reach 
}