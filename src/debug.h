#ifndef ZEPTOS_DEBUG_H
#define ZEPTOS_DEBUG_H

void debug_init(void);
void debug_writec(char c);
void debug_writes(const char *s);
void debug_printf(const char *fmt, ...);

#endif
