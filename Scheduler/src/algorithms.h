#ifndef algorithms_h_
#define algorithms_h_

#ifndef PCRE2_CODE_UNIT_WIDTH
#define PCRE2_CODE_UNIT_WIDTH 8
#endif
#include <pcre2.h>

#include "socket_client.h"
#include "system_config.h"
#include "job_info.h"
#include "resource_info.h"

typedef struct {
	int value;
	unsigned int min_avail;
	unsigned int index;
	bool found;
} best_fit;

void all_to_largest(socket_client *client);
void best_first(socket_client *client);
bool resources_available(resource_info server, resource_info job);
bool schedule_job(socket_client *client, job_info job, server_info server);
bool is_best_fit(best_fit bf, server_info server, job_info job);

#endif
