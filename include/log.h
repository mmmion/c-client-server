#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <pthread.h>
#include "enum_utils.h"

extern const char* MAIN_SOURCE;

void print_colored_label(LogSource source, FILE *stream);

void new_cli_input(void);
void log_msg(LogSource source, const char *fmt, ...);
void log_error(LogSource source, const char *fmt, ...);

#endif