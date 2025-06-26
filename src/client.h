#ifndef CLIENT_H
#define CLIENT_H

#include <netinet/in.h>

#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT 8000

typedef struct s_Client {
    char ip[16];
    int port;
    int sock;
    struct sockaddr_in addr;
} Client;

Client setup_client(void);
int send_message(Client *client, const char *msg);
ssize_t receive_message(Client *client, char *buffer, size_t size);
void close_client(Client *client);

#endif // CLIENT_H
