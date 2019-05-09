#pragma once
#ifndef resource_info_h_
#define resource_info_h_

#ifdef __cplusplus
#ifndef EXTERN_C
#define EXTERN_C
#define EXTERN_C_resource_info_h_
extern "C" {
#endif
#else
#define noexcept
#include <stdbool.h>
#endif

typedef struct resource_info {
	unsigned cores; // CPU cores
	unsigned memory; // memory in MB
	unsigned disk; // disk in MB
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