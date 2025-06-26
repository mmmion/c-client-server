#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#include "enum_utils.h"

void print_colored_label(LogSource source, FILE *stream);
void log_msg(LogSource source, const char *fmt, ...);
void log_error(LogSource source, const char *fmt, ...);

#endif