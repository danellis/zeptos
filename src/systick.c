#include "stm32f4.h"
#include "debug.h"

__attribute__((interrupt)) void systick_handler(void) {
    debug_writes("*");
    GPIOA_ODR ^= 1 << 5;
}
