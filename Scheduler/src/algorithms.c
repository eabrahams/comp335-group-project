#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "algorithms.h"
#include "stringhelper.h"
#include "socket_client.h"
#include "system_config.h"
#include "job_info.h"

/* This function does everything each algorithm needs except for choosing the server to
 * run a given job. That task is given to the funtion pointer called 'algorithm' in this
 * function. In addition to not duplicating code, this allows us to test a scheduling 
 * algorithm without needing a network socket client */
void run_algorithm(socket_client *client, server_info *(*algorithm)(system_config*, server_group*, job_info)) {
	// use regexes instead of sscanf
	regex_info *job_regex = regex_init(JOB_REGEX); // free this once finished

	system_config *config = parse_config("system.xml"); // need to free

	while (true) {
		client_send(client, "REDY");
		char *resp = client_receive(client); // need to free
		if (strncmp(resp, "NONE", 4) == 0)
			break;
		job_info job = strtojob(resp, job_regex); // do not free
		free(resp);

		if (!update_config(config, client)) {
			fprintf(stderr, "unable to updated server information for job %lu\n", job.id);
		}

		// needs to be freed
		server_group *avail_servers = updated_servers_by_avail(config, client, job.req_resc);
		server_info *choice = algorithm(config, avail_servers, job); // do not free

		if (!choice) {
			fprintf(stderr, "unable to find server for job %lu\n", job.id);
			break;
		}

		char *schd = create_schd_str(job.id, choice->type->name, choice->id); // need to free
		free_group(avail_servers);
		bool success = client_msg_resp(client, schd, "OK");
		free(schd);
		if (!success)
			break;
	}

	client_send(client, "QUIT");
	free_config(config);
	regex_free(job_regex);
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

	server_info *best; // a pointer to the server we will be using
	bool best_found = false; // track if we have found an available server
	size_t best_fitness = ULONG_MAX; // the fitness of the server pointed to by best
	size_t i;
	for (i = 0; i < config->num_servers; i++) {
		server_info *server = &config->servers[i];
		if (server->state == SS_UNAVAILABLE) // only consider available servers
			continue;
		/* job_can_run() and job_fitness() take a job_info pointer and a resource_info
		 * struct so we can decide whether we are comparing fitness with a server's
		 * available resources or max resources without duplicating code */
		const resource_info *resc;
		if (!best_found) {
			/* If a server with available resources hasn't been found yet,
			 * we don't need to compare the fitness of an available server
			 * with any other server, so we can skip the rest of the loop
			 * if a server with avaiable resources is found at this point. */
			if (job_can_run(&job, server->avail_resc)) {
				best = server;
				best_fitness = job_fitness(&job, server->avail_resc);
				best_found = true;
				continue;
			}

			/* If no server with available resources has been found, then we compare
			 * max resources, otherwise we compare available resources */
			resc = &server->type->max_resc;
		} else {
			resc = &server->avail_resc;
		}

		/* this is where we compare the fitness of the current server to the fitness of
		 * the server pointed to by best */
		if (job_can_run(&job, *resc)) {
			size_t fitness = job_fitness(&job, *resc);
			if (!best || fitness < best_fitness || (fitness == best_fitness && server->avail_time < best->avail_time)) {
				best = server;
				best_fitness = fitness;
			}
		}
	}

	/* It is possible that best is still not assigned a value at this point,
	 * so it will be necessary for the calling function to handle that case */
	return best;
}

