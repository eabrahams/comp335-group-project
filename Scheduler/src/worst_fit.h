#pragma once
#ifndef worst_fit_h_
#define worst_fit_h_

#include "system_config.h"
#include "job_info.h"

#ifdef __cplusplus
extern "C" {
#else
#define noexcept(BOOL)
#include <stdbool.h>
#endif

void worst_fit(system_config *config, const job_info *job) noexcept(true);

#ifdef __cplusplus
}
#else
#undef noexcept
#endif

#endif