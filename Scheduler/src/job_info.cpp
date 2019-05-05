#include "job_info.h"
#include <type_traits>
static_assert(std::is_pod_v<job_info>, "job_info MUST be POD to be C-compatible!");

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <stdexcept>

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
