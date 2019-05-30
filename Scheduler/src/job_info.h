#pragma once
#ifndef job_info_h_
#define job_info_h_

#include "resource_info.h"

#ifdef __cplusplus
#include "cpp_util.h"
#ifndef EXTERN_C
#define EXTERN_C
#define EXTERN_C_job_info_h_
extern "C" {
#endif
#else
#define noexcept
#include <stdbool.h>
#endif

#include <stdint.h>

typedef struct job_info {
	uintmax_t submit_time; // time the job was submitted to the client
	uintmax_t id; // unique identifier for the job
	uintmax_t est_runtime; // esimated runtime of the job (milliseconds)
	resource_info req_resc; // resources required to run the job
	bool finished;
#ifdef __cplusplus
	// whether a job can run on given resources
	bool can_run(const resource_info &resc) const noexcept;
	// the fitness for a job on given resources
	intmax_t fitness(const resource_info &resc) const noexcept;
#endif
} job_info;

// whether a job can run on given resources, wraps job_info.can_run
bool job_can_run(const job_info *job, const resource_info resc) noexcept;

// the fitness for a job on given resources, wraps job_info.fitness
intmax_t job_fitness(const job_info *job, const resource_info resc) noexcept;

job_info *job_by_id(job_info *jobs, size_t num_jobs, uintmax_t id);

#ifdef __cplusplus
#ifdef EXTERN_C_job_info_h_
}
#undef EXTERN_C_job_info_h_
#undef EXTERN_C
#endif
#else
#undef noexcept
#endif

#endif
