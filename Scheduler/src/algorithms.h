#ifndef algorithms_h_
#define algorithms_h_

#ifndef PCRE2_CODE_UNIT_WIDTH
#define PCRE2_CODE_UNIT_WIDTH 8
#endif
#include <pcre2.h>

#include "socket_client.h"
#include "system_config.h"
#include "job_info.h"

typedef struct {
	int value;
	unsigned int min_avail;
	unsigned int index;
	bool found;
} best_fit;

void all_to_largest(socket_client *client);
server_info *best_server(server_group *avail_servers, job_info job);
void best_first(socket_client *client);
bool schedule_job(socket_client *client, job_info job, server_info server);

#endif
