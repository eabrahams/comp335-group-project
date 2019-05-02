#include <string.h>
#include <stdlib.h>
#include <argp.h>
#include <stdbool.h>

#include "socket_client.h"
#include "algorithms.h"

typedef enum { ALL_TO_LARGEST, FIRST_FIT, BEST_FIRST, WORST_FIT } algorithm;

void usage(char *prog);
algorithm get_algorithm(char *name);

int main(int argc, char **argv) {
	char *algstr;
	int i;
	for (i = 1; i < argc; i++) {
		if (strlen(argv[i]) < 2 || argv[i][0] != '-')
			usage(argv[0]);

		switch(argv[i][1]) {
			case 'a':
				if (i + 1 >= argc)
					usage(argv[0]);
				algstr = argv[++i];
				break;
			case 'h':
				usage(argv[0]);
				break;
		}
	}
	algorithm a = get_algorithm(algstr);

	socket_client *client = client_init(LOCALHOST, DEFAULT_PORT);
	switch(a) {
		case ALL_TO_LARGEST:
			all_to_largest(client);
			break;
		case FIRST_FIT:
			puts("not yet implemented");
			break;
		case BEST_FIRST:
			best_first(client);
			break;
		case WORST_FIT:
			puts("not yet implemented");
			break;
	}
	// update_server(server_of_type(config, type) + id, state, time, cores, memory, disk);
	return 0;
}

algorithm get_algorithm(char *name) {
	algorithm a = ALL_TO_LARGEST;
	// name must be ff, bf or wf so length must be 2 and second char must be f
	if (name && strlen(name) == 2 && name[1] == 'f') {
		switch(name[0]) {
			case 'f':
				a = FIRST_FIT;
				break;
			case 'b':
				a = BEST_FIRST;
				break;
			case 'w':
				a = WORST_FIT;
				break;
		}
	}

	return a;
}

void usage(char *prog) {
	printf("%s%s\n", prog, " [-a ALGORITHM]");
	exit(1);
}

