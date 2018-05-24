.syntax unified
.thumb

.equ GPIOA_BASE, 0x40020000
.equ GPIOA_MODER, GPIOA_BASE + 0x00
.equ GPIOA_OTYPER, GPIOA_BASE + 0x04
.equ GPIOA_PUPDR, GPIOA_BASE + 0x0c
.equ GPIOA_ODR, GPIOA_BASE + 0x14

.equ GPIOC_BASE, 0x40020800
.equ GPIOC_MODER, GPIOC_BASE + 0x00

.equ RCC_BASE, 0x40023800

.equ RCC_CR, RCC_BASE + 0x00
.equ RCC_CR_HSEON, 1 << 16
.equ RCC_CR_HSERDY, 1 << 17
.equ RCC_CR_PLLON, 1 << 24
.equ RCC_CR_PLLRDY, 1 << 25

.equ RCC_PLLCFGR, RCC_BASE + 0x04
.equ RCC_PLLCFGR_PLLM_BIT, 0
.equ RCC_PLLCFGR_PLLN_BIT, 6
.equ RCC_PLLCFGR_PLLP_BIT, 16
.equ RCC_PLLCFGR_PLLSRC, 1 << 22
.equ RCC_PLLCFGR_PLLQ_BIT, 24

.equ RCC_CFGR, RCC_BASE + 0x08
.equ RCC_CFGR_SW_BIT, 0
.equ RCC_CFGR_SWS_BIT, 2

.equ RCC_AHB1ENR, RCC_BASE + 0x30
.equ RCC_AHB1ENR_GPIOAEN, 1 << 0

.equ FLASH_BASE, 0x40023C00
.equ FLASH_ACR, FLASH_BASE + 0x00
.equ FLASH_ACR_LATENCY_BIT, 0
.equ FLASH_ACR_PRFTEN, 1 << 8

.equ NVIC_ISER0, 0xE000E100

.equ SYST_CSR, 0xe000e010
.equ SYST_CSR_COUNTFLAG, 1 << 16

.equ SYST_RVR, 0xe000e014
.equ SYST_CVR, 0xe000e018
.equ SYST_CALIB, 0x0e000e01c

.text

// Blinky function
.thumb_func
.global test
reset_handler:

    // Copy .data section
    ldr r0, =__data_init_start
    ldr r1, =__data_start
    ldr r2, =__data_end

copy_data_section:
    ldr r3, [r0]!
    str r3, [r1]!
    cmp r1, r2
    bne copy_data_section

    // Copy .bss section
    mov r0, #0
    ldr r1, =__bss_start
    ldr r2, =__bss_end

copy_bss_section:
    str r0, [r1]!
    cmp r1, r2
    bne copy_bss_section

    b init

// Vector table
.align 2
.section vectors
    .word __stack_top
    .word reset_handler
    .word panic // NMI
    .word panic // Hard fault
    .word panic // Memory management
    .word panic // Bus fault
    .word panic // Usage fault
    .word 0 // Reserved
    .word 0 // Reserved
    .word 0 // Reserved
    .word 0 // Reserved
    .word 0 // SVC
    .word 0 // Debug monitor
    .word 0 // Reserved
    .word 0 // PendSV
    .word 0 // SysTick
