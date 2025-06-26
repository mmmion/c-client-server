#ifndef CLIENT_CLI_H
#define CLIENT_CLI_H

#include "enum_utils.h"
#include "client.h"

#define PREFIX "/"
#define N_COMMANDS 10
#define AVAILABLE_COMMANDS 3 // Change every time new command gets added

typedef void (*CommandHandler)(const char *args); // Pointer to a void function that takes args { }

typedef struct s_Command {
    char name[20];
    UserPrivilege requirement;
    CommandHandler handler;
} Command;

extern const Command commands[N_COMMANDS];

int handle_input(Client *client);
void handle_client_cli(const char *input);

#endif 
