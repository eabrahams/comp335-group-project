#include <stdlib.h>
#include <string.h>

#include "socket_client.h"
#include "system_config.h"
#include "job_info.h"

void all_to_largest(socket_client *client) {
	system_config *config = parse_config("system.xml");
	unsigned int i;
	unsigned int largest_index = 0;

	/* find largest server
	 * start at 1 because we have already set largest_index to 0, and we will be
	 * comparing the type at index i to the one at largest_index */
	for (i = 1; i < config->num_types; i++)
		if (config->types[i]->cores > config->types[largest_index]->cores)
			largest_index = i;

	char *name = config->types[largest_index]->name;
	
	/* start receiving jobs and send them all to largest server */
	while (true) {
		client_send(client, "REDY");
		char *resp = client_receive(client);
		if (strncmp(resp, "NONE", 4) == 0)
			break;

		job_info j = job_from_string(resp);
		free(resp);

		size_t len = snprintf(NULL, 0, "%s %d %s %d", "SCHD", j.id, name, 0) + 1;
		char *schd_str[len];
		snprintf(schd_str, len, "%s %d %s %d", "SCHD", j.id, name, 0);

		bool scheduled = client_msg_resp(client, schd_str, "OK");
		if (!scheduled) {
			fprintf("%s%d%s\n", "Unable to schedule job ", j.id, " with command: ", schd_str);
			exit(1);
		}
	}

	client_send("QUIT");
}

