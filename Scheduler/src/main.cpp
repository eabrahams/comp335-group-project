#include "socket_client.h"
#include "system_config.h"

void allToLargest(socket_client *client) {
	system_config config = parse_config("system.xml");
	unsigned int i;
	unsigned int largest_index = 0;
	for (i = 1; i < config->num_types; i++) {
		if (config->types[i]->cores > config->types[largest_index]->cores)
			largest_index = i;
	}
}

int main(int argc, char **argv) {
	socket_client *client = client_init(LOCALHOST, DEFAULT_PORT);
	allToLargest(client);
	// update_server(server_of_type(config, type) + id, state, time, cores, memory, disk);
	return 0;
}
