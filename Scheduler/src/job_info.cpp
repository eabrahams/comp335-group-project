#include "job_info.h"
#include "cpp_util.h"
ASSERT_IS_POD(job_info);

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include <limits>

bool job_info::can_run(const resource_info &resc) const noexcept {
	return req_resc <= resc;
}

intmax_t job_info::fitness(const resource_info &resc) const noexcept {
	return static_cast<intmax_t>(resc.cores) - static_cast<intmax_t>(req_resc.cores);
	// worst_fit algorithm actually requires fitness for non-runnable jobs
	//if(can_run(resc)) return static_cast<int>(resc.cores) - static_cast<int>(req_resc.cores);
	//else throw std::invalid_argument("Job must be able to run on resources to calculate a fitness value");
}

bool job_can_run(const job_info *job, const resource_info resc) noexcept {
	// effectively just a delegate to the operator overload, but accessible from C
	return job->can_run(resc);
};

intmax_t job_fitness(const job_info *job, const resource_info resc) noexcept {
	// worst_fit algorithm actually requires fitness for non-runnable jobs
	try {

		return job->fitness(resc);

	} catch(...) {

		return std::numeric_limits<intmax_t>::min();
	}
};
