#pragma once
#ifndef job_info_h_
#define job_info_h_

#include "resource_info.h"

#ifdef __cplusplus
extern "C" {
#else
#define noexcept(BOOL)
#include <stdbool.h>
#endif

typedef struct {
	unsigned submit_time;
	int id;
	unsigned est_runtime;
	resource_info req_resc;
	bool complete;
} job_info;

job_info job_from_string(const char *jobstr) noexcept(true);

// whether a job can run on given resources
bool job_can_run(const job_info *job, const resource_info resc) noexcept(true);

// the fitness for a job on given resources
int job_fitness(const job_info *job, const resource_info resc) noexcept(true);

#ifdef __cplusplus
}
#else
#undef noexcept
#endif

#endif