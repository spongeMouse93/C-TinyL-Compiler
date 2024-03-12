#include "utils.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

int err(const char *level, const char *func, int line, const char *fmt, ...) {
  char s[1024];
  va_list va;
  va_start(va, fmt);
  vsprintf(s, fmt, va);
  va_end(va);
  return fprintf(stderr, "[%s:%s:%d] %s", level, func, line, s);
}
