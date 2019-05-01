#ifndef socket_client_h_
#define socket_client_h_

#include <sys/socket.h>
#include <stdbool.h>

#define LOCALHOST "127.0.0.1"
#define DEFAULT_PORT 8096

typedef struct {
	struct sockaddr_in *socket;
	int fd;
} socket_client;

socket_client *client_init(char *host, int port);
void client_send(socket_client *client, const char *msg);
bool client_msg_resp(socket_client *client, const char *msg, const char *expected_response);
char *client_receive(socket_client *client);


#endif
