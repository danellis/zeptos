#include <stdbool.h>
#include "stm32f4.h"
#include "debug.h"

void led(bool on) {
    GPIOA_ODR = on ? 1 << 5 : 0;
}

void init(void) {
    // Enable GPIOA peripheral clock
    RCC_AHB1ENR = RCC_AHB1ENR_GPIOAEN;

    // Set PA5 to output
    GPIOA_MODER = 0b01 << 10;

    // Turn on HSE
    RCC_CR |= RCC_CR_HSEON;
    while ((RCC_CR & RCC_CR_HSERDY) == 0);

    // Set flash wait cycles
    FLASH_ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN | (2 << FLASH_ACR_LATENCY_BIT);
    
    // Configure PLL
    // System clock = 84MHz, PLL48CLK = 48MHz
    RCC_PLLCFGR = (RCC_PLLCFGR & RCC_PLLCFGR_RESERVED_MASK)
        | RCC_PLLCFGR_PLLSRC // Set HSE as PLL source
        | (8 << RCC_PLLCFGR_PLLM_BIT) // M = 8
        | (336 << RCC_PLLCFGR_PLLN_BIT) // N = 336
        | (1 << RCC_PLLCFGR_PLLP_BIT) // P = 4
        | (7 << RCC_PLLCFGR_PLLQ_BIT) // Q = 7
    ;

    // Turn on PLL
    RCC_CR |= RCC_CR_PLLON;
    while ((RCC_CR & RCC_CR_PLLRDY) == 0);

    RCC_CFGR = 
        (2 << RCC_CFGR_SW_BIT) // Use PLL as clock source
        | (0b100 << RCC_CFGR_PPRE1_BIT) // Prescale APB1 clock /2
    ;

    // Wait for clock source to change
    while ((RCC_CFGR & (2 << RCC_CFGR_SWS_BIT)) == 0);

    debug_init();

    for (;;) {
        debug_putc('H'); debug_putc('I');
    }

    // Set SysTick doing its funky thang
    SYST_CVR = SYST_RVR = (84000000 / 8 / 100) - 1; // 100Hz
    SYST_CSR = 0b001;

    for (;;) {
        while ((SYST_CSR & SYST_CSR_COUNTFLAG) == 0);
        GPIOA_ODR ^= 1 << 5;
    }
}
