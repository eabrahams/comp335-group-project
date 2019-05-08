#ifndef socket_client_h_
#define socket_client_h_

#include <sys/socket.h>
#include <stdbool.h>
#include "job_info.h"

//#include "system_config.h"

#define LOCALHOST "127.0.0.1"
#define DEFAULT_PORT 8096

typedef struct socket_client {
	struct sockaddr_in *socket;
	int fd;
} socket_client;

typedef struct server_info server_info;
typedef struct system_config system_config;
typedef struct server_group server_group;

socket_client *client_init(char *host, int port);
void client_send(socket_client *client, const char *msg);
bool client_msg_resp(socket_client *client, const char *msg, const char *expected_response);
char *client_receive(socket_client *client);

void client_run_algorithm(socket_client*,server_info*(*alg)(system_config*,server_group*,job_info));


#endif
