#include "resource_info.h"

bool operator<(const resource_info &lhs, const resource_info &rhs) noexcept(true) {
	return lhs != rhs && lhs <= rhs;
}

bool operator<=(const resource_info &lhs, const resource_info &rhs) noexcept(true) {
	return lhs.cores <= rhs.cores && lhs.memory <= rhs.memory && lhs.disk <= rhs.disk;
};

bool operator>(const resource_info &lhs, const resource_info &rhs) noexcept(true) {
	return lhs != rhs && lhs >= rhs;
};

bool operator>=(const resource_info &lhs, const resource_info &rhs) noexcept(true) {
	return lhs.cores >= rhs.cores && lhs.memory >= rhs.memory && lhs.disk >= rhs.disk;
};

bool operator==(const resource_info &lhs, const resource_info &rhs) noexcept(true) {
	return lhs.cores == rhs.cores && lhs.memory == rhs.memory && lhs.disk == rhs.disk;
};

bool operator!=(const resource_info &lhs, const resource_info &rhs) noexcept(true) {
	return lhs.cores != rhs.cores || lhs.memory != rhs.memory || lhs.disk != rhs.disk;
};