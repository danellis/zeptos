#include "stm32f4.h"

#include <stdbool.h>

void led(bool on) {
    GPIOA_ODR = on ? 1 << 5 : 0;
}

void init(void) {
    // Enable GPIOA peripheral clock
    RCC_AHB1ENR = RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;

    // Set PA5 to output and PA8 to MCO1
    GPIOA_MODER = 0b01 << 10 | 0b10 << 16;
    GPIOA_OSPEEDR |= 3 << 16;

    // Set PC9 MCO2
    GPIOC_MODER = 0b10 << 18;

    // Turn on HSE
    RCC_CR |= RCC_CR_HSEON;
    while ((RCC_CR & RCC_CR_HSERDY) == 0);

    // Set flash wait cycles
    FLASH_ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN | (2 << FLASH_ACR_LATENCY_BIT);
    
    // Configure PLL
    // System clock = 84MHz, PLL48CLK = 48MHz
    RCC_PLLCFGR = (RCC_PLLCFGR & RCC_PLLCFGR_RESERVED_MASK)
        | RCC_PLLCFGR_PLLSRC // Set HSE as PLL source
        | (4 << RCC_PLLCFGR_PLLM_BIT) // M = 8
        | (168 << RCC_PLLCFGR_PLLN_BIT) // N = 336
        | (1 << RCC_PLLCFGR_PLLP_BIT) // P = 4
        | (7 << RCC_PLLCFGR_PLLQ_BIT) // Q = 7
    ;

    // Turn on PLL
    RCC_CR |= RCC_CR_PLLON;
    while ((RCC_CR & RCC_CR_PLLRDY) == 0);

    RCC_CFGR = 
        (2 << RCC_CFGR_SW_BIT) // Use PLL as clock source
        | (0b11 << 21) // Output PLL on MCO1
        | (0b10 << 30) // Output HSE on MCO2
    ;

    // Wait for clock source to change
    while ((RCC_CFGR & (2 << RCC_CFGR_SWS_BIT)) == 0);

    led(true);

    // Set SysTick doing its funky thang
    SYST_CVR = SYST_RVR = (84000000 / 8 / 100) - 1; // 100Hz
    SYST_CSR = 0b001;

    for (;;) {
        while ((SYST_CSR & SYST_CSR_COUNTFLAG) == 0);
        GPIOA_ODR ^= 1 << 5;
    }
}
