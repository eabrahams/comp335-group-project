#pragma once
#ifndef job_info_h_
#define job_info_h_

#include "resource_info.h"

#ifdef __cplusplus
extern "C" {
#else
#define noexcept
#include <stdbool.h>
#endif

typedef struct job_info {
	unsigned submit_time;
	unsigned id;
	unsigned est_runtime;
	resource_info req_resc;
#ifdef __cplusplus
	bool can_run(const resource_info &resc) const noexcept;
	int fitness(const resource_info &resc) const;
#endif
} job_info;

job_info job_from_string(const char *jobstr) noexcept;

// whether a job can run on given resources
bool job_can_run(const job_info *job, const resource_info resc) noexcept;

// the fitness for a job on given resources
int job_fitness(const job_info *job, const resource_info resc) noexcept;

#ifdef __cplusplus
}
#else
#undef noexcept
#endif

#endif
