.syntax unified
.cpu cortex-m0plus
.thumb

.section .text
.global asm_add
.type asm_add, %function

asm_add:
    ADD r0, r0, r1
    BX lr