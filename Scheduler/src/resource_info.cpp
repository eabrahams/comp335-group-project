#include "resource_info.h"
#include "cpp_util.h"
ASSERT_IS_POD(resource_info);

bool resource_info::operator<(const resource_info &rhs) const noexcept {
	return this->operator!=(rhs) && this->operator<=(rhs);
}

bool resource_info::operator<=(const resource_info &rhs) const noexcept {
	return cores <= rhs.cores && memory <= rhs.memory && disk <= rhs.disk;
};

bool resource_info::operator>(const resource_info &rhs) const noexcept {
	return this->operator!=(rhs) && this->operator>=(rhs);
};

bool resource_info::operator>=(const resource_info &rhs) const noexcept {
	return cores >= rhs.cores && memory >= rhs.memory && disk >= rhs.disk;
};

bool resource_info::operator==( const resource_info &rhs) const noexcept {
	return cores == rhs.cores && memory == rhs.memory && disk == rhs.disk;
};

bool resource_info::operator!=(const resource_info &rhs) const noexcept {
	return cores != rhs.cores || memory != rhs.memory || disk != rhs.disk;
};