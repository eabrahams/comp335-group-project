#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "algorithms.h"
#include "stringhelper.h"
#include "socket_client.h"
#include "system_config.h"
#include "job_info.h"

void run_algorithm(socket_client *client, server_info *(*algorithm)(system_config*, server_group*, job_info)) {
	regex_info *job_regex = regex_init(JOB_REGEX);
	system_config *config = parse_config("system.xml");

	while (true) {
		client_send(client, "REDY");
		char *resp = client_receive(client);
		if (strncmp(resp, "NONE", 4) == 0)
			break;
		
		job_info job = strtojob(resp, job_regex);
		free(resp);

		if (!update_config(config, client)) {
			fprintf(stderr, "unable to updated server information for job %d\n", job.id);
		}

		server_group *avail_servers = updated_servers_by_avail(config, client, job.req_resc);
		server_info *choice = algorithm(config, avail_servers, job);
		free_group(avail_servers);

		if (!choice) {
			fprintf(stderr, "unable to find server for job %d\n", job.id);
			break;
		}

		char *schd = create_schd_str(job.id, choice->type->name, choice->id);
		bool success = client_msg_resp(client, schd, "OK");
		if (!success)
			break;
	}

	client_send(client, "QUIT");
	free_config(config);
}

server_info *all_to_largest(system_config *config, server_group *group, job_info job) {
	const server_type *largest_type = &config->types[0];
	size_t i;
	for (i = 1; i < config->num_types; i++)
		if (largest_type->max_resc.cores < config->types[i].max_resc.cores)
			largest_type = &config->types[i];

	server_info *largest = start_of_type(config, largest_type);
	if (!job_can_run(&job, largest->type->max_resc) || !group)
		return NULL;
	return largest;
}

server_info *first_fit(system_config *config, server_group *group, job_info job) {
	puts("not yet implemented");
	return NULL;
}
server_info *best_fit(system_config *config, server_group *group, job_info job) {
	puts("not yet implemented");
	return NULL;
}
/*
server_info *worst_fit(system_config *config, server_group *group, job_info job) {
	puts("not yet implemented");
	return NULL;
}*/

