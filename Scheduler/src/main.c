#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "socket_client.h"
#include "system_config.h"
#include "job_info.h"
#include "all_to_largest.h"

void usage(char *name);

server_info *first_fit(system_config *config, server_group *group, job_info job);
server_info *best_fit(system_config *config, server_group *group, job_info job);
server_info *worst_fit(system_config *config, server_group *group, job_info job);

server_info *first_fit(system_config *config, server_group *group, job_info job) {
	puts("not implemented here");
	return NULL;
}
server_info *best_fit(system_config *config, server_group *group, job_info job) {
	puts("not implemented here");
	return NULL;
}
server_info *worst_fit(system_config *config, server_group *group, job_info job) {
	puts("not implemented here");
	return NULL;
}

int main(int argc, char **argv) {
	socket_client *client = client_init(LOCALHOST, DEFAULT_PORT);

	void *algorithm = &all_to_largest;

	int i;
	for (i = 0; i < argc; i++) {
		if (argv[i][0] == '-') {
			switch(argv[i][1]) {
				case 'a':
					i++;
					if (strcmp(argv[i], "ff") == 0)
						algorithm = &first_fit;
					else if (strcmp(argv[i], "bf") == 0)
						algorithm = &best_fit;
					else if (strcmp(argv[i], "wf") == 0)
						algorithm = &worst_fit;
					else
						fprintf(stderr, "algorithm not implemented: %s\n", argv[i]);
					break;
				default:
					usage(argv[0]);
			}
		} else {
			usage(argv[0]);
		}
	}

	client_run_algorithm(client, algorithm);

	// update_server(server_of_type(config, type) + id, state, time, cores, memory, disk);
	return 0;
}

void usage(char *name) {
	printf("%s%s\n", name, " [-a ALGORITHM]");
	exit(1);
}
