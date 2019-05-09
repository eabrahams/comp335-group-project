#pragma once
#ifndef worst_fit_h_
#define worst_fit_h_

#ifdef __cplusplus
#ifndef EXTERN_C
#define EXTERN_C
#define EXTERN_C_worst_fit_h_
extern "C" {
#endif
#else
#define noexcept(BOOL)
#include <stdbool.h>
#endif

#include "algorithms.h"

server_info *worst_fit(system_config* config, server_group *candidates, job_info job);

#ifdef __cplusplus
#ifdef EXTERN_C_worst_fit_h_
#undef EXTERN_C_worst_fit_h_
#undef EXTERN_C
}
#endif
#else
#undef noexcept
#endif

#endif