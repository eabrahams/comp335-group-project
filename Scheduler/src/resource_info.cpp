#include "resource_info.h"
#include <type_traits>
static_assert(std::is_pod_v<resource_info>, "resource_info MUST be POD to be C-compatible!");

bool resource_info::operator<(const resource_info &rhs) const noexcept(true) {
	return this->operator!=(rhs) && this->operator!=(rhs);
}

bool resource_info::operator<=(const resource_info &rhs) const noexcept(true) {
	return cores <= rhs.cores && memory <= rhs.memory && disk <= rhs.disk;
};

bool resource_info::operator>(const resource_info &rhs) const noexcept(true) {
	return this->operator!=(rhs) && this->operator>=(rhs);
};

bool resource_info::operator>=(const resource_info &rhs) const noexcept(true) {
	return cores >= rhs.cores && memory >= rhs.memory && disk >= rhs.disk;
};

bool resource_info::operator==( const resource_info &rhs) const noexcept(true) {
	return cores == rhs.cores && memory == rhs.memory && disk == rhs.disk;
};

bool resource_info::operator!=(const resource_info &rhs) const noexcept(true) {
	return cores != rhs.cores || memory != rhs.memory || disk != rhs.disk;
};