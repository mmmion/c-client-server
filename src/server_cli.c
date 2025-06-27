#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "server_cli.h"
#include "log.h"

void server_cmd_help(const char *args, ClientInfo *clients, int count) {
    (void)args;
    (void)clients;
    (void)count;
    log_msg(COMMAND, "Available server commands:");
    log_msg(COMMAND, "- /help : Show this help message");
    log_msg(COMMAND, "- /list : List connected clients");
    log_msg(COMMAND, "- /kick <id> : Disconnect client with given ID");
}

void server_cmd_list(const char *args, ClientInfo *clients, int count) {
    (void)args;
    log_msg(SERVER, "Connected clients:");
    for (int i = 0; i < count; i++) {
        log_msg(SERVER, "- ID: %d | Socket: %d", clients[i].id, clients[i].socket);
    }
}

void server_cmd_kick(const char *args, ClientInfo *clients, int count) {
    int id = atoi(args);
    if (id <= 0) {
        log_msg(SERVER, "Invalid ID");
        return;
    }
    for (int i = 0; i < count; i++) {
        if (clients[i].id == id) {
            close(clients[i].socket);
            log_msg(SERVER, "Kicked client ID %d", id);
            return;
        }
    }
    log_msg(SERVER, "Client with ID %d not found", id);
}

const ServerCommand server_commands[SERVER_COMMANDS] = {
    {"help", server_cmd_help},
    {"list", server_cmd_list},
    {"kick", server_cmd_kick},
};

static const char *skip_spaces(const char *str) {
    while (*str == ' ') str++;
    return str;
}

void handle_server_cli(const char *input, ClientInfo *clients, int client_count) {
    if (input[0] != PREFIX[0]) {
        log_msg(COMMAND, "Not a server command.");
        return;
    }

    const char *command_line = input + 1;

    char cmd_name[20] = {0};
    int i = 0;
    while (command_line[i] && command_line[i] != ' ' && i < 19) {
        cmd_name[i] = command_line[i];
        i++;
    }
    cmd_name[i] = '\0';

    const char *args = skip_spaces(command_line + i);

    for (int c = 0; c < SERVER_COMMANDS; c++) {
        if (strcmp(server_commands[c].name, cmd_name) == 0) {
            server_commands[c].handler(args, clients, client_count);
            return;
        }
    }

    log_msg(SERVER, "Unknown command: %s", cmd_name);
}
