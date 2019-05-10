#pragma once
#ifndef resource_info_h_
#define resource_info_h_

#ifdef __cplusplus
#include "cpp_util.h"
#ifndef EXTERN_C
#define EXTERN_C
#define EXTERN_C_resource_info_h_
extern "C" {
#endif
#else
#define noexcept
#include <stdbool.h>
#endif

#include <stdint.h>

typedef struct resource_info {
	uintmax_t cores; // CPU cores
	uintmax_t memory; // memory in MB
	uintmax_t disk; // disk in MB
} resource_info;

// utilities for resource capacity comparison logic
#ifdef __cplusplus
bool operator<(const resource_info &lhs, const resource_info &rhs) noexcept;
bool operator<=(const resource_info &lhs, const resource_info &rhs) noexcept;
bool operator>(const resource_info &lhs, const resource_info &rhs) noexcept;
bool operator>=(const resource_info &lhs, const resource_info &rhs) noexcept;
bool operator==(const resource_info &lhs, const resource_info &rhs) noexcept;
bool operator!=(const resource_info &lhs, const resource_info &rhs) noexcept;
#endif

#ifdef __cplusplus
#ifdef EXTERN_C_resource_info_h_
}
#undef EXTERN_C_resource_info_h_
#undef EXTERN_C
#endif
#else
#undef noexcept
#endif

#endif