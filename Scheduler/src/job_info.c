#include <string.h>
#include "job_info.h"
#include "resource_info.h"

job_info job_from_string(char *jobstr) {
	job_info j;
	if (strncmp(jobstr, "JOBN", 4) != 0) {
		fprintf(stderr, "%s%s\n", "Bad job string: ", jobstr);
		exit(1);
	}

	sscanf(jobstr + 5, "%d %d %d %d %d %d",
			j->submit_time,
			j->id,
			j->runtime,
			j->req_resc.cores,
			j->req_resc.memory,
			j->req_resc.disk);
	return j;
}

