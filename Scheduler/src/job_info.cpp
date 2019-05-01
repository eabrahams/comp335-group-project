#include "job_info.h"

// whether a job can run on given resources
bool job_can_run(const job_info *job, const resource_info resc) noexcept(true) {
	return job->req_resc <= resc;
};

// the fitness for a job on given resources
int job_fitness(const job_info *job, const resource_info resc) noexcept(true) {
	if(job_can_run(job, resc)) return resc.cores - job->req_resc.cores;
	else return -1;
};