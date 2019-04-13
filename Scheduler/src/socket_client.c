#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <string.h>

#include "socket_client.h"

#define LOCALHOST "127.0.0.1"
#define DEFAULT_PORT 8096
#define OK "OK"
#define END "."
#define VERBOSE

socket_client *client_init(char *host, int port) {
	socket_client *client = malloc(sizeof *client);
	client->socket = malloc(sizeof *client->socket);
	client->fd = socket(AF_INET, SOCK_STREAM, 0);
	client->socket->sin_family = AF_INET;
	client->socket->sin_port = htons(port);
	inet_pton(AF_INET, host, &client->socket->sin_addr);
	int connect_status = connect(client->fd, (struct sockaddr *)client->socket, sizeof(*client->socket));
	if (connect_status < 0) {
		fprintf(stderr, "%s\n", "Connection Failed");
		exit(1);
	}
	if (!client_msg_resp(client, "HELO", OK))
		exit(1);
	if (!client_msg_resp(client, "AUTH comp335", OK))
		exit(1);
	return client;
}

void client_send(socket_client *client, char *msg) {
	send(client->fd, msg, strlen(msg), 0);
}

char *client_receive(socket_client *client) {
	char *buffer = malloc(sizeof *buffer * 1024);
	int length = read(client->fd, buffer, 1023);
	buffer[length] = '\0';
	char *msg = malloc(sizeof *msg * length);
	strncpy(msg, buffer, length);
	free(buffer);
	return msg;
}

bool client_msg_resp(socket_client *client, char *msg, char *expected_response) {
	bool result = true;
	client_send(client, msg);
	char *response = client_receive(client);
	if (!strcmp(response, expected_response)) {
		fprintf(stderr, "%s%s\n", "server sent incorrect response: ", response);
		result = false;
	}
	free(response);

	return result;
}

