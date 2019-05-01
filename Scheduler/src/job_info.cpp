#include "job_info.h"
#include <type_traits>
static_assert(std::is_pod_v<job_info>, "job_info MUST be POD to be C-compatible!");

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

bool job_info::can_run(const resource_info &resc) const noexcept(true) {
	return req_resc <= resc;
}

int job_info::fitness(const resource_info &resc) const noexcept(false) {
	if(can_run(resc)) return resc.cores - req_resc.cores;
	else throw nullptr; // actual exception doesn't matter, and this is quite fast
}

bool job_can_run(const job_info *job, const resource_info resc) noexcept(true) {
	// effectively just a delegate to the operator overload, but accessible from C
	return job->can_run(resc);
};

int job_fitness(const job_info *job, const resource_info resc) noexcept(true) {
	try {
		return job->fitness(resc);
	} catch(...) {
		return -1;
	}
};