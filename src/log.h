#ifndef LOG_H
#define LOG_H

typedef enum {
    SERVER,
    CLIENT,
    INPUT,
    OTHER
} LogSource;

void log_msg(LogSource source, const char *fmt, ...);
void log_error(LogSource source, const char *fmt, ...);

#endif