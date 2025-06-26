#include "log.h"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "termcolor_c.h" 
static const char *get_source_name(LogSource source) {
    switch (source) {
        case SERVER: return "SERVER";
        case CLIENT: return "CLIENT";
        case INPUT:  return "INPUT";
        case COMMAND:  return "COMMAND";
        default:     return "OTHER";
    }
}

static void set_label_color(LogSource source, FILE *stream) {
    switch (source) {
        case SERVER: text_magenta(stream); break;  
        case CLIENT: text_cyan(stream);    break;
        case INPUT:  text_green(stream);   break;
        case COMMAND: text_blue(stream);   break;
        default:     text_yellow(stream);  break;
    }
}

void print_colored_label(LogSource source, FILE *stream) {
    const char *label = get_source_name(source);

    set_label_color(source, stream);

    fputs("[", stream);
    fputs(label, stream);
    fputs("]", stream);

    reset_colors(stream);

    text_white(stream);

    fputs(" ", stream);
}

void log_msg(LogSource source, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    print_colored_label(source, stdout);
    vprintf(fmt, args);
    printf("\n");

    reset_colors(stdout);
    va_end(args);
}

void log_error(LogSource source, const char *fmt, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    fputs("[", stderr);
    set_label_color(source, stderr);
    fputs(get_source_name(source), stderr);
    reset_colors(stderr);
    fprintf(stderr, " ERROR] %s: ", buffer);

    perror(NULL); 
    reset_colors(stderr);
}
