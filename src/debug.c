#include <stdbool.h>
#include "stm32f4.h"

void debug_init(void) {
    RCC_APB1ENR |= RCC_APB1ENR_USART2EN; // Enable peripheral clock for USART2

    GPIOA_MODER |= 0b1010 << 4; // PA2, PA3 to AF
    GPIOA_OSPEEDR |= 0b1111 << 4; // PA2, PA3 to high speed
    GPIOA_PUPDR |= 0b0101 << 4; // Pull-up
    GPIOA_AFRL |= (7 << 8) | (13 << 12); // Set PA2, PA3 to AF7 (USART2)

    // Baud rate 115k2 with PCLK = 42MHz)
    USART2_BRR = 42000000/115200;

    USART2_CR1 |= USART_CR1_TE | USART_CR1_UE;
}

void debug_writec(char c) {
    while ((USART2_SR & USART_SR_TXE) == 0);
    USART2_DR = c;
}

void debug_writes(const char *s) {
    while (*s) {
        debug_writec(*s++);
    }
}
