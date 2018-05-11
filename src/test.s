.syntax unified
.thumb

.equ GPIOA_BASE, 0x40020000
.equ GPIOA_MODE, GPIOA_BASE + 0x00
.equ GPIOA_OTYPER, GPIOA_BASE + 0x04
.equ GPIOA_PUPDR, GPIOA_BASE + 0x0c
.equ GPIOA_ODR, GPIOA_BASE + 0x14

.equ RCC_BASE, 0x40023800
.equ RCC_AHB1ENR, RCC_BASE + 0x30
.equ GPIOAEN, 1 << 0

.text

// Blinky function
.thumb_func
.global test
test:
    // Enable GPIOA peripheral clock
    ldr r1, =RCC_AHB1ENR
    ldr r2, =GPIOAEN
    str r2, [r1]

    // Set PA5 to output
    ldr r1, =GPIOA_MODE
    ldr r2, =0b01 << 10
    str r2, [r1]

    ldr r1, =GPIOA_ODR
    ldr r2, =0

loop:
    eor r2, r2, 1 << 5
    str r2, [r1]
    mov r0, #0x180000
wait:
    subs r0, r0, #1
    bne wait
    b loop

// Vector table
.align 2
.section vectors
    .word 0
    .word test
