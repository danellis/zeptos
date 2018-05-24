#include <stdbool.h>
#include <stdarg.h>
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

static const char lower_digits[] = "0123456789abcdef";
static const char upper_digits[] = "0123456789ABCDEF";

static const char *format_integer(char *end, unsigned int i, int base, bool upper) {
    const char *digits = upper ? upper_digits : lower_digits;

    *--end = 0;

    if (i == 0) {
        *--end = '0';
    } else {
        while (i) {
            *--end = digits[i % base];
            i /= base;
        }
    }

    return end;
}

static void debug_vprintf(const char *fmt, va_list args) {
    char c, type, color;
    char buffer[21], *buffer_end = buffer + sizeof(buffer) - 1;
    const char *start;
    *buffer_end = 0;

    while ((c = *fmt++)) {
        if (c == '%') {
            if ((type = *fmt++) == 0) break;
            switch (type) {
                case 'c':
                    debug_writec(va_arg(args, int));
                    break;

                case 's':
                    start = va_arg(args, const char *);
                    debug_writes(start ? start : "(null)");
                    break;

                case 'i':
                    start = format_integer(buffer_end, va_arg(args, unsigned int), 10, false);
                    debug_writes(start);
                    break;

                case 'x':
                    start = format_integer(buffer_end, va_arg(args, unsigned int), 16, false);
                    debug_writes(start);
                    break;

                case 'X':
                    start = format_integer(buffer_end, va_arg(args, unsigned int), 16, true);
                    debug_writes(start);
                    break;

                case 'p':
                    for (int i = 28; i >= 0; i -= 4) {
                        debug_writec(lower_digits[((uint32_t) va_arg(args, void *) >> i) & 0xf]);
                    }
                    break;

                default:
                    debug_writec(type);
                    break;
            }
        } else if (c == '~') {
            if ((type = *fmt++) == 0 || (color = *fmt++) == 0) break;
            debug_writes("\e[");
            if (type == 'F') debug_writec('3');
            if (type == 'B') debug_writec('4');
            debug_writec(color);
            debug_writec('m');
        } else {
            debug_writec(c);
        }
    }
}

void debug_printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    debug_vprintf(fmt, args);
    va_end(args);
}
