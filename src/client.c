#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "log.h"
#include "client.h"
#include "client_cli.h"

#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT 8000

/*******************/
/* INPUT FUNCTIONS */
/*******************/
void input_ip(char *ip) {
    while (1) {
        log_msg(INPUT, "Insert the server IP address [%s]: ", DEFAULT_IP);
        if (!fgets(ip, 16, stdin)) {
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
        log_msg(INPUT, "Insert the server port [%d]: ", DEFAULT_PORT);
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

Client setup_client() {
    Client new_client;

    input_ip(new_client.ip);
    input_port(&new_client.port);

    memset(&new_client.addr, 0, sizeof(new_client.addr));
    new_client.addr.sin_family = AF_INET;
    new_client.addr.sin_port = htons(new_client.port);
    inet_pton(AF_INET, new_client.ip, &new_client.addr.sin_addr);

    return new_client;
}

int send_message_to_server(Client *client, const char *msg) {
    ssize_t sent = send(client->sock, msg, strlen(msg), 0);
    if (sent < 0) {
        log_error(CLIENT, "Failed to send message");
        return -1;
    }
    return 0;
}

ssize_t receive_message(Client *client, char *buffer, size_t size) {
    ssize_t bytes_received = recv(client->sock, buffer, size - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
    }
    return bytes_received;
}

void close_client(Client *client) {
    if (client->sock >= 0) {
        close(client->sock);
        client->sock = -1;
    }
}


/*****************/
/* MAIN FUNCTION */
/*****************/

int main() {
    char buffer[1024];
    Client client = setup_client();

    client.sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client.sock < 0) {
        log_error(CLIENT, "Socket error");
        exit(1);
    }
    log_msg(CLIENT, "TCP client socket created.");

    if (connect(client.sock, (struct sockaddr*)&client.addr, sizeof(client.addr)) < 0) {
        log_error(CLIENT, "Connect failed");
        close_client(&client);
        exit(1);
    }
    log_msg(CLIENT, "Connected to server at %s:%d", client.ip, client.port);
    while (1) {
        if (handle_input(&client) == -1)
            break;

        // Try to receive any messages from server without blocking
        ssize_t bytes_received = recv(client.sock, buffer, sizeof(buffer) - 1, MSG_DONTWAIT);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            log_msg(CLIENT, "Received from server: %s", buffer);
        } else if (bytes_received == 0) {
            log_msg(CLIENT, "Server closed the connection.");
            break;
        } else {
            // bytes_received < 0 and errno == EWOULDBLOCK or EAGAIN means no data, ignore
        }
    }

    close_client(&client);
    return 0;
}