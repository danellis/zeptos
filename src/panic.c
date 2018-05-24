#include "debug.h"

__attribute__((interrupt)) void panic(void) {
    debug_writes("PANIC!\n");
    for (;;);
}
