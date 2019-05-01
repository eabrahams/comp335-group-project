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
#include "socket_client.h"

void worst_fit(socket_client *client) noexcept(true);

#ifdef __cplusplus
}
#else
#undef noexcept
#endif

#endif