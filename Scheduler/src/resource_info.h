#pragma once
#ifndef resource_info_h_
#define resource_info_h_

#ifdef __cplusplus
extern "C" {
#else
#define noexcept(BOOL)
#include <stdbool.h>
#endif

typedef struct {
	unsigned cores; // CPU cores
	unsigned memory; // memory in MB
	unsigned disk; // disk in MB
} resource_info;

#ifdef __cplusplus
}

bool operator<(const resource_info &lhs, const resource_info &rhs) noexcept(true);
bool operator<=(const resource_info &lhs, const resource_info &rhs) noexcept(true);
bool operator>(const resource_info &lhs, const resource_info &rhs) noexcept(true);
bool operator>=(const resource_info &lhs, const resource_info &rhs) noexcept(true);
bool operator==(const resource_info &lhs, const resource_info &rhs) noexcept(true);
bool operator!=(const resource_info &lhs, const resource_info &rhs) noexcept(true);
#else
#undef noexcept
#endif

#endif