#include "resource_info.h"
#include <type_traits>
static_assert(std::is_pod_v<resource_info>, "resource_info MUST be POD to be C-compatible!");

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