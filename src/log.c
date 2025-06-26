#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

static const char *get_source_name(LogSource source) {
    switch (source) {
        case SERVER: return "SERVER";
        case CLIENT: return "CLIENT";
        case INPUT:  return "INPUT";
        default:     return "OTHER";
    }
}

void log_msg(LogSource source, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf("[%s] ", get_source_name(source));
    vprintf(fmt, args);  
    printf("\n");        
    va_end(args);
}

void log_error(LogSource source, const char *fmt, ...) {
    char buffer[1024]; 
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args); 
    va_end(args);

    char err[1060]; 
    snprintf(err, sizeof(err), "[%s ERROR] %s", get_source_name(source), buffer);
    perror(err); 
}
