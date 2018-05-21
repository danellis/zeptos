#include <stdbool.h>
#include "stm32f4.h"

extern void led(bool);

void debug_init(void) {
    GPIOA_MODER |= 0b1010 << 4; // PA2, PA3 to AF
    GPIOA_OSPEEDR |= 0b1111 << 4; // PA2, PA3 to high speed
    GPIOA_AFRL |= (7 << 8) | (7 << 12); // Set PA2, PA3 to AF7 (USART2)
    RCC_APB1ENR |= RCC_APB1ENR_USART2EN; // Enable peripheral clock for USART2

    // Baud rate 115k2 with SYSCLK 84MHz (BRR = 45.5625)
    // USART2_BRR = (45 << 4) | 9;

    USART2_CR1 = USART_CR1_UE | USART_CR1_TE;
}

void debug_putc(char c) {
    while ((USART2_SR & USART_SR_TC) == 0);
    USART2_DR = c;
}
