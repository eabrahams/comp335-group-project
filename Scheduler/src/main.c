#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "socket_client.h"
#include "system_config.h"
#include "algorithms.h"

void usage(char *name);

int main(int argc, char **argv) {
	//server_info *(*algorithm)(system_config*,server_group*,job_info) = &all_to_largest;
	algorithm_t a = ALL_TO_LARGEST;

	int i;
	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			switch(argv[i][1]) {
				case 'a':
					i++;
					if (strcmp(argv[i], "ff") == 0)
						a = FIRST_FIT;
						//algorithm = &first_fit;
					else if (strcmp(argv[i], "bf") == 0)
						a = BEST_FIT;
						//algorithm = &best_fit;
					else if (strcmp(argv[i], "wf") == 0)
						a = WORST_FIT;
						//algorithm = &worst_fit;
					else if (strcmp(argv[i], "bg") == 0)
						a = BEST_GUESS;
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

	socket_client *client = client_init(LOCALHOST, DEFAULT_PORT);

	//run_algorithm(client, algorithm);
	run_algorithm(client, a);

	return 0;
}

void usage(char *name) {
	printf("%s%s\n", name, " [-a ALGORITHM]");
	exit(1);
}

