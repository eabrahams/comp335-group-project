#include "socket_client.h"
#include "system_config.h"

int main(int argc, char **argv) {
	socket_client *client = client_init(LOCALHOST, DEFAULT_PORT);
	allToLargest(client);
	// update_server(server_of_type(config, type) + id, state, time, cores, memory, disk);
	return 0;
}
