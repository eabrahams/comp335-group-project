#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <string.h>

#include "socket_client.h"
#include "system_config.h"
#include "job_info.h"
#include "stringhelper.h"

#define LOCALHOST "127.0.0.1"
#define DEFAULT_PORT 8096
#define OK "OK"
#define END "."
#define VERBOSE
#define BUF_SIZE 1024
#define JOB_REGEX "JOBN ([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+)"

bool client_msg_resp(socket_client *client, const char *msg, const char *expected_response);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * creates and prepares socket to communicate with server  *
 * greeting and authentication is handled here. The next   *
 * message to be sent is "REDY", after which the server    *
 * will start sending jobs.								   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
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

/* sends a null-terminated string to the server over
 * a socket */
void client_send(socket_client *client, const char *msg) {
	send(client->fd, msg, strlen(msg), 0);
}

/* returns a string that is sent by the server. */
char *client_receive(socket_client *client) {
	char *buffer = malloc(sizeof *buffer * BUF_SIZE);
	int length = read(client->fd, buffer, BUF_SIZE - 1);
	buffer[length] = '\0';
	char *msg = calloc(length + 1, sizeof *msg);
	strncpy(msg, buffer, length);
	free(buffer);
	return msg;
}

/* Sends a message and then checks if the response is the one expected.
 * If we know exactly what the response should be then use this.
 * Because there is an issue where the client is receiving more data at
 * the end of a string, this function only tests n number of characters,
 * where n=strlen(expected_response). */
bool client_msg_resp(socket_client *client, const char *msg, const char *expected_response) {
	size_t len = strlen(expected_response);
	bool result = true;
	client_send(client, msg);
	char *response = client_receive(client);
	if (strncmp(response, expected_response, len) != 0) {
		fprintf(stderr, "expected \"%s\" of size %lu but received \"%s\" of size %lu in response to \"%s\" of size %lu\n", expected_response, len, response, strlen(response), msg, strlen(msg));
		//fprintf(stderr, "%s%s%s%s%c\n", "expected \"", expected_response, "\" but received \"", response, '"');
		result = false;
	}
	free(response);

	return result;
}

void client_free(socket_client *client) {
	free(client->socket);
	free(client);
}

