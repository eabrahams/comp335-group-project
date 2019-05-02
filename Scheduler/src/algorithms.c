#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>

#include "algorithms.h"
#include "socket_client.h"
#include "system_config.h"
#include "job_info.h"
#include "resource_info.h"

/* calculate the difference between two numbers */
#define difference(x,y) ((((x) >= (y)) ? (x) : (y)) - (((x) <= (y)) ? (x) : (y)))
#define SCHD_FORMAT "%s %d %s %d"

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

		job_info j = job_from_string(resp);
		free(resp); // we don't need the response string anymore

		/* snprintf returns the number of characters it wants to write, so we
		 * call it with null and size 0 to get that number, and then add 1
		 * because snprintf writes a null character to the end of the string. */
		size_t len = snprintf(NULL, 0, "%s %d %s %d", "SCHD", j.id, name, 0) + 1;
		char schd_str[len];
		snprintf(schd_str, len, "%s %d %s %d", "SCHD", j.id, name, 0);

		bool success = client_msg_resp(client, schd_str, "OK");
		if (!success) {
			fprintf(stderr, "%s%d%s%s\n", "Unable to schedule job ", j.id, " with command: ", schd_str);
			exit(1);
		}
	}

	client_send(client, "QUIT");

	free_config(config);
}

bool resources_available(resource_info server, resource_info job) {
	return (server.cores >= job.cores &&
			server.memory >= job.memory &&
			server.disk >= job.disk);
}

bool is_best_fit(best_fit bf, server_info server, job_info job) {
	/* all values we're dealing with here are unsigned so we risk an integer
	 * underflow if we simply subtract one from the other. We can't use abs()
	 * because there is no sign to remove, so instead we have to do a few extra
	 * steps to get the correct value (check the difference macro at the top) */
	uint32_t fitness = difference(job.req_resc.cores, server.avail_resc.cores);
	//unsigned int fitness = abs(job.req_resc.cores - server.avail_resc.cores);
	return (fitness < bf.value ||
			(fitness == bf.value && server.avail_time < bf.min_avail));
}

void best_first(socket_client *client) {
	//unsigned int best_fit, min_avail;
	//best_fit = min_avail = UINT_MAX;
	
	system_config *config = parse_config("system.xml");
	while (true) {
		client_send(client, "REDY");
		char *resp = client_receive(client);
		if (strncmp(resp, "NONE", 4) == 0)
			break;

		job_info job = job_from_string(resp);
		free(resp);

		best_fit bf = { UINT_MAX, UINT_MAX, 0, false };
		unsigned int i, fitness;
		for (i = 0; i < config->num_servers; i++) {
			resource_info server_resc = config->servers[i].avail_resc;
			if (!resources_available(server_resc, job.req_resc))
				continue;

			if (is_best_fit(bf, config->servers[i], job)) {
				bf.value = fitness;
				bf.min_avail = config->servers[i].avail_time;
				bf.index = i;
				bf.found = true;
			}
		}

		if (bf.found) {
			bool success = schedule_job(client, &job, &config->servers[i]);
			if (!success) {
				exit(1);
			}
		} else {
			fputs("best not found", stderr);
		}
	}

	free_config(config);
}

bool schedule_job(socket_client *client, job_info *job, server_info *server) {
	size_t len = snprintf(NULL, 0, SCHD_FORMAT,
			"SCHD",
			job->id,
			server->type->name,
			server->id) + 1;
	char schd[len];
	snprintf(schd, len, SCHD_FORMAT,
			"SCHD",
			job->id,
			server->type->name,
			server->id);
	return client_msg_resp(client, schd, "OK");
}

