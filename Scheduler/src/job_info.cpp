#include "job_info.h"
#include "cpp_util.h"
ASSERT_IS_POD(job_info);

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <stdexcept>

/* create job struct from string
 * format of string is:
 * JOBN {SUBMIT_TIME} {ID} {RUN_TIME} {CORES} {MEMORY} {DISK} 
 */
job_info job_from_string(const char *jobstr) noexcept {
	if(strncmp(jobstr, "JOBN", 4) != 0) {
		fprintf(stderr, "%s%s\n", "Bad job string: ", jobstr);
		exit(1);
	}

	job_info j;
	// The first 5 characters are "JOBN ", which we don't need
	sscanf(jobstr + 5, "%d %d %d %d %d %d",
		&j.submit_time,
		&j.id,
		&j.est_runtime,
		&j.req_resc.cores,
		&j.req_resc.memory,
		&j.req_resc.disk);
	return j;
}

bool job_info::can_run(const resource_info &resc) const noexcept {
	return req_resc <= resc;
}

int job_info::fitness(const resource_info &resc) const {
	if(can_run(resc)) return static_cast<int>(resc.cores) - static_cast<int>(req_resc.cores);
	else throw std::invalid_argument("Job must be able to run on resources to calculate a fitness value");
}

bool job_can_run(const job_info *job, const resource_info resc) noexcept {
	// effectively just a delegate to the operator overload, but accessible from C
	return job->can_run(resc);
};

int job_fitness(const job_info *job, const resource_info resc) noexcept {
	try {
		return job->fitness(resc);
	} catch(...) {
		return -1;
	}
};
