#ifndef ENUM_UTILS_H
#define ENUM_UTILS_H

#include <string.h>

typedef enum {
    SERVER,
    CLIENT,
    INPUT,
    COMMAND,
    OTHER
} LogSource;

typedef enum {
    USER,
    ADMIN
} UserPrivilege;

static inline const char* logsource_to_string(LogSource source) {
    switch (source) {
        case SERVER:   return "SERVER";
        case CLIENT:   return "CLIENT";
        case INPUT:    return "INPUT";
        case COMMAND:  return "COMMAND";
        case OTHER:    return "OTHER";
        default:       return "UNKNOWN";
    }
}

static inline LogSource string_to_logsource(const char* str) {
    if (strcmp(str, "SERVER") == 0) return SERVER;
    else if (strcmp(str, "CLIENT") == 0) return CLIENT;
    else if (strcmp(str, "INPUT") == 0) return INPUT;
    else if (strcmp(str, "COMMAND") == 0) return COMMAND;
    else if (strcmp(str, "OTHER") == 0) return OTHER;
    else return -1; 
}


static inline const char* privilege_to_string(UserPrivilege p) {
    switch (p) {
        case USER:   return "USER";
        case ADMIN:  return "ADMIN";
        default:     return "UNKNOWN";
    }
}

#endif