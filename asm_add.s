.syntax unified
.cpu cortex-m33
.thumb

.section .text
.global asm_add
.type asm_add, %function

.thumb_func
asm_add:
    ADD r0, r0, r1
    BX lr