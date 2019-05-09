#pragma once
#ifndef worst_fit_h_
#define worst_fit_h_

#ifdef __cplusplus
extern "C" {
#else
#define noexcept(BOOL)
#include <stdbool.h>
#endif

// MUST go here so it gets wrapped exactly once by extern "C" {..}
#include "algorithms.h"

//server_info *worst_fit(system_config* config, server_group *candidates, job_info job);

#ifdef __cplusplus
}
#else
#undef noexcept
#endif

#endif