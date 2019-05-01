#include "job_info.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>

job_info job_from_string(const char *jobstr) noexcept(true) {
	job_info j;
	if(strncmp(jobstr, "JOBN", 4) != 0) {
		fprintf(stderr, "%s%s\n", "Bad job string: ", jobstr);
		exit(1);
	}

	sscanf(jobstr + 5, "%d %d %d %d %d %d",
		&j.submit_time,
		&j.id,
		&j.est_runtime,
		&j.req_resc.cores,
		&j.req_resc.memory,
		&j.req_resc.disk);
	return j;
}

bool job_can_run(const job_info *job, const resource_info resc) noexcept(true) {
	return job->req_resc <= resc;
};

int job_fitness(const job_info *job, const resource_info resc) noexcept(true) {
	if(job_can_run(job, resc)) return resc.cores - job->req_resc.cores;
	else return -1;
};