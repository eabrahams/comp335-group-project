#include "system_config.h"
#include "socket_client.h"
#include "job_info.h"

#include <stdio.h>

void all_to_largest(socket_client *client) {
	system_config *config = parse_config("system.xml");
	unsigned int i;
	unsigned int largest_index = 0;

	/* find largest server
	 * start at 1 because we have already set largest_index to 0, and we will be
	 * comparing the type at index i to the one at largest_index */
	for (i = 1; i < config->num_types; i++)
		if (config->types[i].max_resc.cores > config->types[largest_index].max_resc.cores)
			largest_index = i;

	char *name = config->types[largest_index].name;
	
	/* start receiving jobs and send them all to largest server */
	while (true) {
		client_send(client, "REDY");
		char *resp = client_receive(client);
		if (strncmp(resp, "NONE", 4) == 0)
			break;

		job_info *j = job_from_string(resp);

		size_t len = snprintf(NULL, 0, "%s %d %s %d", "SCHD", j->id, name, 0) + 1;
		char *schd_str[len];
		snprintf(schd_str, len, "%s %d %s %d", "SCHD", j->id, name, 0);

		client_send(client, schd_str);
		free(resp);
		resp = client_receive(client);
		if (strncmp(resp, "OK", 2) != 0) {
			fprintf(stderr, "%s%s\n", "Expected 'OK', but got: ", resp);
			exit(1);
		}
		free(resp);
	}

	client_send(client, "QUIT");
}

