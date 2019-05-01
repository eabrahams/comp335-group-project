#include "socket_client.h"
#include "all_to_largest.h"
#include "worst_fit.h"

int main(int argc, char **argv) {
	socket_client *client = client_init(LOCALHOST, DEFAULT_PORT);
	all_to_largest(client);
	// update_server(server_of_type(config, type) + id, state, time, cores, memory, disk);
	return 0;
}
