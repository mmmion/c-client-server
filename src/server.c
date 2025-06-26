#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "log.h"

#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT 8000

typedef struct s_Server {
    int port; 
    char ip[16];               // Store IP as fixed size array for safety
    int server_socket;
    struct sockaddr_in server_addr;
    socklen_t addr_size;
} Server;

/******************/
/*   PROTOTYPES   */
/******************/
void input_ip(char *ip, size_t ip_size);
void input_port(int *port);

Server setup_server(void);

/*******************/
/* INPUT FUNCTIONS */
/*******************/
void input_ip(char *ip, size_t ip_size) {
    while (1) {
        log_msg(INPUT, "Insert the IP address [%s]: ", DEFAULT_IP);
        if (!fgets(ip, ip_size, stdin)) {
            log_msg(INPUT, "Error reading input.");
            continue;
        }
        ip[strcspn(ip, "\n")] = 0;

        if (strlen(ip) == 0) {
            strcpy(ip, DEFAULT_IP);
        }

        struct sockaddr_in sa;
        int result = inet_pton(AF_INET, ip, &(sa.sin_addr));
        if (result == 1) {
            break;
        } else {
            log_msg(INPUT, "Invalid IP address. Please enter a valid IPv4 address.");
        }
    }
}

void input_port(int *port) {
    char buffer[100];
    while (1) {
        log_msg(INPUT, "Insert the port [%d]: ", DEFAULT_PORT);
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            log_msg(INPUT, "Error reading input.");
            continue;
        }
        buffer[strcspn(buffer, "\n")] = 0;

        if (strlen(buffer) == 0) {
            *port = DEFAULT_PORT;
            break;
        }

        if (sscanf(buffer, "%d", port) == 1 && *port > 0 && *port < 65536) {
            break;
        } else {
            log_msg(INPUT, "Invalid port. Please enter a number between 1 and 65535.");
        }
    }
}

Server setup_server() {
    Server new_server;

    input_ip(new_server.ip, sizeof(new_server.ip));

    input_port(&new_server.port);


    new_server.addr_size = sizeof(new_server.server_addr);
    memset(&new_server.server_addr, 0, sizeof(new_server.server_addr));
    new_server.server_addr.sin_family = AF_INET;
    new_server.server_addr.sin_port = htons(new_server.port);
    inet_pton(AF_INET, new_server.ip, &new_server.server_addr.sin_addr);

    return new_server;
}

/*****************/
/* MAIN FUNCTION */
/*****************/
int main() {
    char buffer[1024];
    Server server = setup_server();

    server.server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server.server_socket < 0) {
        log_error(SERVER, "Socket creation failed");
        exit(1);
    }
    log_msg(SERVER, "TCP server socket created.");

    if (bind(server.server_socket, (struct sockaddr*)&server.server_addr, sizeof(server.server_addr)) < 0) {
        log_error(SERVER, "Bind failed");
        close(server.server_socket);
        exit(1);
    }
    log_msg(SERVER, "Bind to the port number: %d", server.port);

    if (listen(server.server_socket, 5) < 0) {
        log_error(SERVER, "Listen failed");
        close(server.server_socket);
        exit(1);
    }
    log_msg(SERVER, "Listening...");

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addr_size = sizeof(client_addr);

        int client_socket = accept(server.server_socket, (struct sockaddr*)&client_addr, &addr_size);
        if (client_socket < 0) {
            log_error(SERVER, "Accept failed");
            continue;
        }
        log_msg(SERVER, "Client connected.");

        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        while (bytes_received > 0) 
        {
            bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
            log_msg(SERVER, "Received %d bytes", bytes_received);
        }

        if (bytes_received == 0) {
            log_msg(SERVER, "Client disconnected.");
        } else if (bytes_received < 0) {
            log_msg(SERVER, "Recv error.");
        } 

        close(client_socket); 
    }

    close(server.server_socket);
    return 0;
}
