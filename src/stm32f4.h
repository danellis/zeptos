#ifndef ZEPTOS_STM32F4_H
#define ZEPTOS_STM32F4_H

#include <stdint.h>

#define REGISTER(addr) (*(volatile uint32_t *)(addr))

#define GPIOA_BASE 0x40020000
#define GPIOA_MODER REGISTER(GPIOA_BASE + 0x00)
#define GPIOA_OTYPER REGISTER(GPIOA_BASE + 0x04)
#define GPIOA_OSPEEDR REGISTER(GPIOA_BASE + 0x08)
#define GPIOA_PUPDR REGISTER(GPIOA_BASE + 0x0c)
#define GPIOA_ODR REGISTER(GPIOA_BASE + 0x14)

#define GPIOC_BASE 0x40020800
#define GPIOC_MODER REGISTER(GPIOC_BASE + 0x00)

#define RCC_BASE 0x40023800

#define RCC_CR REGISTER(RCC_BASE + 0x00)
#define RCC_CR_HSEON (1 << 16)
#define RCC_CR_HSERDY (1 << 17)
#define RCC_CR_PLLON (1 << 24)
#define RCC_CR_PLLRDY (1 << 25)

#define RCC_PLLCFGR REGISTER(RCC_BASE + 0x04)
#define RCC_PLLCFGR_RESERVED_MASK 0b11110000101111001000000000000000
#define RCC_PLLCFGR_PLLM_BIT 0
#define RCC_PLLCFGR_PLLN_BIT 6
#define RCC_PLLCFGR_PLLP_BIT 16
#define RCC_PLLCFGR_PLLSRC (1 << 22)
#define RCC_PLLCFGR_PLLQ_BIT 24

#define RCC_CFGR REGISTER(RCC_BASE + 0x08)
#define RCC_CFGR_SW_BIT 0
#define RCC_CFGR_SWS_BIT 2
#define RCC_CFGR_MCO1PRE_BIT 24
#define RCC_CFGR_MCO2PRE_BIT 27

#define RCC_AHB1ENR REGISTER(RCC_BASE + 0x30)
#define RCC_AHB1ENR_GPIOAEN (1 << 0)
#define RCC_AHB1ENR_GPIOCEN (1 << 2)

#define FLASH_BASE 0x40023C00
#define FLASH_ACR REGISTER(FLASH_BASE + 0x00)
#define FLASH_ACR_LATENCY_BIT 0
#define FLASH_ACR_PRFTEN (1 << 8)
#define FLASH_ACR_ICEN (1 << 9)
#define FLASH_ACR_DCEN (1 << 10)

#define NVIC_ISER0 0xE000E100

#define SYST_CSR REGISTER(0xE000E010)
#define SYST_CSR_COUNTFLAG (1 << 16)

#define SYST_RVR REGISTER(0xE000E014)
#define SYST_CVR REGISTER(0xE000E018)

#endif