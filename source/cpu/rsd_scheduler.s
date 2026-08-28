.syntax unified
.cpu cortex-m33
.thumb

.extern active_task
.extern task_sp
.extern scheduler_running
.extern task_pick_next

.section .text.isr_pendsv, "ax", %progbits
.global isr_pendsv
.thumb_func
isr_pendsv:
    ldr  r0, =scheduler_running
    ldrb r1, [r0]
    cbz  r1, .Lfirst_run

    push {r4-r11, lr}
    ldr  r0, =active_task
    ldrb r1, [r0]
    ldr  r2, =task_sp
    lsls r3, r1, #2
    add  r2, r2, r3
    str  sp, [r2]

    bl   task_pick_next
    b    .Lload

.Lfirst_run:
    movs r1, #1
    strb r1, [r0]

.Lload:
    ldr  r0, =active_task
    ldrb r1, [r0]
    ldr  r2, =task_sp
    lsls r3, r1, #2
    add  r2, r2, r3
    ldr  sp, [r2]

    pop  {r4-r11, lr}
    bx   lr 