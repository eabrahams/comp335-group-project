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
			fprintf(stderr, "unable to updated server information for job %lu\n", job.id);
		}

		server_group *avail_servers = updated_servers_by_avail(config, client, job.req_resc);
		server_info *choice = algorithm(config, avail_servers, job);

		if (!choice) {
			fprintf(stderr, "unable to find server for job %lu\n", job.id);
			break;
		}

		char *schd = create_schd_str(job.id, choice->type->name, choice->id);
		free_group(avail_servers);
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
	if (!config || !group)
		fprintf(stderr, "config or group not defined for job %lu\n", job.id);
	puts("not yet implemented");
	return NULL;
}

server_info *best_fit(system_config *config, server_group *group, job_info job) {
	if (!config || !group) {
		fprintf(stderr, "%s%lu\n", "config or group not defined for job ", job.id);
		return NULL;
	}
	server_info *best;
	bool best_found = false;
	size_t best_fitness = ULONG_MAX;
	size_t i;
	for (i = 0; i < config->num_servers; i++) {
		server_info *server = &config->servers[i];
		if (server->state == SS_UNAVAILABLE)
			continue;
		const resource_info *resc;
		if (!best_found) {
			if (job_can_run(&job, server->avail_resc)) {
				best = server;
				best_fitness = job_fitness(&job, server->avail_resc);
				best_found = true;
				continue;
			}

			resc = &server->type->max_resc;
		} else {
			resc = &server->avail_resc;
		}
		if (job_can_run(&job, *resc)) {
			size_t fitness = job_fitness(&job, *resc);
			if (!best || fitness < best_fitness || (fitness == best_fitness && server->avail_time < best->avail_time)) {
				best = server;
				best_fitness = fitness;
			}
		}
	}
	return best;
}

