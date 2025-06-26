#ifndef ENUM_UTILS_H
#define ENUM_UTILS_H

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

static inline const char* privilege_to_string(UserPrivilege p) {
    switch (p) {
        case USER:   return "USER";
        case ADMIN:  return "ADMIN";
        default:     return "UNKNOWN";
    }
}

#endif