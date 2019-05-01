#pragma once
#ifndef resource_info_h_
#define resource_info_h_

#ifdef __cplusplus
extern "C" {
#else
#define noexcept(BOOL)
#include <stdbool.h>
#endif

typedef struct resource_info {
	unsigned cores; // CPU cores
	unsigned memory; // memory in MB
	unsigned disk; // disk in MB
#ifdef __cplusplus
	bool operator<(const resource_info &rhs) const noexcept(true);
	bool operator<=(const resource_info &rhs) const noexcept(true);
	bool operator>(const resource_info &rhs) const noexcept(true);
	bool operator>=(const resource_info &rhs) const noexcept(true);
	bool operator==(const resource_info &rhs) const noexcept(true);
	bool operator!=(const resource_info &rhs) const noexcept(true);
#endif
} resource_info;

#ifdef __cplusplus
}
#else
#undef noexcept
#endif

#endif