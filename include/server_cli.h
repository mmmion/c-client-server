#ifndef SERVER_CLI_H
#define SERVER_CLI_H

#include "enum_utils.h"
#include "server.h"

#define PREFIX "/"
#define SERVER_COMMANDS 4

typedef void (*ServerCommandHandler)(const char *args, ClientInfo *clients, int count);

typedef struct s_ServerCommand {
    char name[20];
    ServerCommandHandler handler;
} ServerCommand;

extern const ServerCommand server_commands[SERVER_COMMANDS];

void handle_server_cli(const char *input, ClientInfo *clients, int client_count);

#endif
