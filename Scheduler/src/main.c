#include "socket_client.h"
#include "system_config.h"

void allToLargest(socket_client *client) {
	system_config *config = parse_config("system.xml");
	unsigned int i;
	unsigned int largest_index = 0;
	// start at 1, since we've set the index to 0
	for (i = 1; i < config->num_types; i++)
		if (config->types[i].max_resc.cores > config->types[largest_index].max_resc.cores)
			largest_index = i;

	while (true) {
		client_send(client, "REDY");
		char *resp = client_receive(client);
		char *start = strtok(resp, " ");
		if (strcmp(start, "JOBN") != 0) {
			break;
		}
	}
}

int main(int argc, char **argv) {
	socket_client *client = client_init(LOCALHOST, DEFAULT_PORT);
	allToLargest(client);
	// update_server(server_of_type(config, type) + id, state, time, cores, memory, disk);
	return 0;
}
