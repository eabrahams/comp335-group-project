#include "resource_info.h"
ASSERT_IS_POD(resource_info);

bool operator<(const resource_info &lhs, const resource_info &rhs)  noexcept {
	return lhs != rhs && lhs <= rhs;
}

bool operator<=(const resource_info &lhs, const resource_info &rhs)  noexcept {
	return lhs.cores <= rhs.cores && lhs.memory <= rhs.memory && lhs.disk <= rhs.disk;
};

bool operator>(const resource_info &lhs, const resource_info &rhs)  noexcept {
	return lhs != rhs && lhs >= rhs;
};

bool operator>=(const resource_info &lhs, const resource_info &rhs)  noexcept {
	return lhs.cores >= rhs.cores && lhs.memory >= rhs.memory && lhs.disk >= rhs.disk;
};

bool operator==(const resource_info &lhs, const resource_info &rhs)  noexcept {
	return lhs.cores == rhs.cores && lhs.memory == rhs.memory && lhs.disk == rhs.disk;
};

bool operator!=(const resource_info &lhs, const resource_info &rhs)  noexcept {
	return lhs.cores != rhs.cores || lhs.memory != rhs.memory || lhs.disk != rhs.disk;
};