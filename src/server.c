#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "log.h"
#include "server.h"

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

int send_message_to_client(int client_socket, const char *msg) {
    ssize_t sent = send(client_socket, msg, strlen(msg), 0);
    if (sent < 0) {
        log_error(SERVER, "Failed to send message");
        return -1;
    }
    return 0;
}

void close_clients(int *client_sockets, int count) {
    for (int i = 0; i < count; i++) {
        close(client_sockets[i]);
    }
}

/*****************/
/* MAIN FUNCTION */
/*****************/
int main() {
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

    if (listen(server.server_socket, MAX_CLIENTS) < 0) {
        log_error(SERVER, "Listen failed");
        close(server.server_socket);
        exit(1);
    }
    log_msg(SERVER, "Listening...");

    ClientInfo clients[MAX_CLIENTS];
    int clients_connected = 0;

    while (1) {
        socklen_t addr_size = sizeof(clients[clients_connected].addr);
        int client_socket = accept(server.server_socket, (struct sockaddr*)&clients[clients_connected].addr, &addr_size);
        if (client_socket < 0) {
            log_error(SERVER, "Accept failed");
            continue;
        }

        if (clients_connected == MAX_CLIENTS) {
            log_msg(SERVER, "Server is full, cannot accept more clients now.");

            const char *msg = "503 Server Full\n";
            send(client_socket, msg, strlen(msg), 0);
            log_msg(SERVER, "Server is full, rejected connection.");
            close(client_socket); 
            sleep(1);
            continue;
        }

        log_msg(SERVER, "Client connected.");
        clients[clients_connected].privilege = USER;
        send_message_to_client(client_socket, "Welcome client.");


        clients[clients_connected].socket = client_socket;
        clients[clients_connected].id = clients_connected+1;
        clients_connected++;
    }
    
    for (int i = 0; i < clients_connected; i++) {
        close(clients[i].socket);
    }
    close(server.server_socket);
    return 0;
}
