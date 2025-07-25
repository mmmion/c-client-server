#include <stdio.h>
#include <string.h>

#include "client_cli.h"
#include "log.h"

void cmd_help(const char *args) {
    (void)args;
    size_t i;
    log_msg(COMMAND, "Available commands:");
    for (i=0; i<N_COMMANDS; i++) {
        log_msg(COMMAND, "- Name: %s : Require : %s", commands[i].name, privilege_to_string(commands[i].requirement));
    }
}

void cmd_echo(const char *args) {
    printf("Echo: %s\n", args);
}

void cmd_admin(const char *args) {
    printf("Admin command executed with args: %s\n", args);
}

const Command commands[N_COMMANDS] = {
    {"help", USER, cmd_help},
    {"echo", USER, cmd_echo},
    {"admin", ADMIN, cmd_admin},
};

static const char *skip_spaces(const char *str) {
    while (*str == ' ') str++;
    return str;
}

int handle_input(Client *client) {
    static int cli_mode = 0;  
    char input_line[1024];

    if (cli_mode) {
        new_cli_input();
    }

    if (!fgets(input_line, sizeof(input_line), stdin)) {
        log_msg(CLIENT, "Error reading input");
        return -1;
    }

    input_line[strcspn(input_line, "\n")] = 0; 

    if (!cli_mode && input_line[0] == PREFIX[0] && input_line[1] == '\0') {
        cli_mode = 1;
        return 0;
    }

    if (cli_mode) {
        handle_client_cli(input_line);
        cli_mode = 0;  
    } else {
        if (send_message_to_server(client, input_line) < 0) {
            log_error(CLIENT, "Failed to send message");
            return -1;
        }
    }

    cli_mode = 0;

    return 0;
}
void handle_client_cli(const char *input) {
    const char *command_line = input;

    char cmd_name[20] = {0};
    int i = 0;
    while (command_line[i] && command_line[i] != ' ' && i < 19) {
        cmd_name[i] = command_line[i];
        i++;
    }
    cmd_name[i] = '\0';

    const char *args = command_line + i;
    args = skip_spaces(args);

    for (int c = 0; c < N_COMMANDS; c++) {
        if (commands[c].handler == NULL) continue;
        if (strcmp(commands[c].name, cmd_name) == 0) {
            commands[c].handler(args);
            return;
        }
    }

    log_msg(CLIENT, "Unknown command: %s\n", cmd_name);
}