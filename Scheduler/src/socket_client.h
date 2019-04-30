#ifndef socket_client_h_
#define socket_client_h_

#include <sys/socket.h>

#define LOCALHOST "127.0.0.1"
#define DEFAULT_PORT 8096

typedef struct {
	struct sockaddr_in *socket;
	int fd;
} socket_client;

socket_client *client_init(char *host, int port);
void client_send(socket_client *client, char *msg);
char *client_receive(socket_client *client);


#endif
