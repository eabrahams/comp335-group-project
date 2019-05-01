#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "best_fit.h"

void best_fit(socket_client *client) {
	unsigned int best_fit, min_avail;
	best_fit = min_avail = UINT_MAX;

	system_config *config = parse_config("system.xml");

	while (true) {
		client_send(client, "REDY");
		char *resp = client_receive(client);
		if (strncmp(resp, "NONE", 4) == 0)
			break;

		job_info j = job_from_string(resp);
		free(resp);
		int fitness;
		unsigned int i;
		for (i = 0; i < config->num_servers; i++) {
		}
	}

	free_config(config);
}
