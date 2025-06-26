#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>

#define MAX_CLIENTS 5
#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT 8000

typedef struct s_Server {
    int port;
    char ip[16];
    int server_socket;
    struct sockaddr_in server_addr;
    socklen_t addr_size;
} Server;

Server setup_server(void);
int send_message(int client_socket, const char *msg);
void close_clients(int *client_sockets, int count);

#endif // SERVER_H
