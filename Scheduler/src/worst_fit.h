#pragma once
#ifndef worst_fit_h_
#define worst_fit_h_

#include "system_config.h"

#ifdef __cplusplus
extern "C" {
#else
#define noexcept(BOOL)
#include <stdbool.h>
#endif

void worst_fit(system_config *config) noexcept(true);

#ifdef __cplusplus
}
#else
#undef noexcept
#endif

#endif