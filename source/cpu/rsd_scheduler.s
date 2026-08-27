.syntax unified
.cpu cortex-m33
.thumb

.extern active_task
.extern task_sp
.extern task_pick_next

.section text

.section .text.yield, "ax", %progbits
.global yield
.thumb_func
yield:
    push {r4-r11, lr}

    ldr  r0, =active_task
    ldrb r1, [r0]
    ldr  r2, =task_sp
    lsls r3, r1, #2
    add  r2, r2, r3
    str  sp, [r2]

    bl task_pick_next

    ldr  r0, =active_task
    ldrb r1, [r0]
    ldr  r2, =task_sp
    lsls r3, r1, #2
    add  r2, r2, r3
    ldr  sp, [r2]

    pop {r4-r11, pc}

.section .text.start_scheduler, "ax", %progbits
.global start_scheduler
.thumb_func
start_scheduler:
    ldr  r0, =active_task
    ldrb r1, [r0]
    ldr  r2, =task_sp
    lsls r3, r1, #2
    add  r2, r2, r3
    ldr  sp, [r2]
    pop  {r4-r11, pc}