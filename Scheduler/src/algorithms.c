#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#include "algorithms.h"
#include "socket_client.h"
#include "system_config.h"
#include "job_info.h"
#include "resource_info.h"
#include "stringhelper.h"

#define SCHD_FORMAT "%s %u %s %d"
#define JOB_REGEX "JOBN ([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+)"

void all_to_largest(socket_client *client) {
	regex_info *job_regex = regex_init(JOB_REGEX);
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

		job_info j = strtojob(resp, job_regex);
		free(resp); // we don't need the response string anymore

		char *schd = create_schd_str(j.id, name, 0);
		bool success = client_msg_resp(client, schd, "OK");
		if (!success) {
			fprintf(stderr, "%s%u%s%s\n", "Unable to schedule job ", j.id, " with command: ", schd);
			exit(1);
		}
		free(schd);
	}

	client_send(client, "QUIT");

	free_config(config);
}

server_info *best_server(server_group *avail_servers, job_info job) {
	server_info *best, *other;
	server_info **current = &other;
	int best_fitness = INT_MAX;
	size_t i;
	for (i = 0; i < avail_servers->num_servers; i++) {
		server_info *server = avail_servers->servers[i];
		if (!best && job_can_run(&job, server->avail_resc)) {
			current = &best;
			best = server;
			best_fitness = job_fitness(&job, server->avail_resc);
			continue;
		}

		resource_info resc = (best) ? server->avail_resc : server->type->max_resc;
		if (job_can_run(&job, resc)) {
			int fitness = job_fitness(&job, resc);
			if (!*current || fitness < best_fitness || (fitness == best_fitness && server->avail_time < (*current)->avail_time)) {
				best_fitness = fitness;
				*current = server;
			}
		}
	}

	return (best) ? best : other;
}
//if (fitness < bf.value || (fitness == bf.value && config->servers[i].avail_time < bf.min_avail)) {

void best_first(socket_client *client) {
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
			fprintf(stderr, "unable to update server information for job %d\n", job.id);
			break;
		}

		/*
		for (size_t i = 0; i < config->num_types; i++) {
			if (!update_servers_by_type(config, client, &config->types[i])) {
				fprintf(stderr, "unable to update server '%s' for job %d\n", config->types[i].name, job.id);
				goto ALGORITHM_END;
			}
		}
		*/

		//best_fit bf = { INT_MAX, UINT_MAX, 0, false };
		server_group *avail_servers = updated_servers_by_avail(config, client, job.req_resc);
		server_info *best = best_server(avail_servers, job);
		if (!best) {
			fprintf(stderr, "best not found for job %d\n", job.id);
			break;
		}
		char *schd = create_schd_str(job.id, best->type->name, best->id);
		bool success = client_msg_resp(client, schd, "OK");
		if (!success)
			break;
		/*
		unsigned int i;
		for (i = 0; i < config->num_servers; i++) {
			resource_info server_resc = config->servers[i].avail_resc;
			if (!job_can_run(&job, server_resc))
				continue;

			int fitness = job_fitness(&job, server_resc);
			if (fitness < bf.value || (fitness == bf.value && config->servers[i].avail_time < bf.min_avail)) {
				bf.value = fitness;
				bf.min_avail = config->servers[i].avail_time;
				bf.index = i;
				bf.found = true;
			}
		}


		bool success = false;
		if (bf.found) {
			char *schd = create_schd_str(job.id, config->servers[bf.index].type->name, config->servers[bf.index].id);
			success = client_msg_resp(client, schd, "OK");
			free(schd);
		} else {
			fprintf(stderr, "best not found for job %d\n", job.id);
		}
		if (!success)
			break;
		*/
	}

	client_send(client, "QUIT");
	free_config(config);
}

