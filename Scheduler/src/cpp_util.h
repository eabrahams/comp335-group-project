#pragma once
#ifndef cpp_util_h_
#define cpp_util_h_

#ifndef __cplusplus
#error "Do NOT include this file in C source!"
#else

#include <cstddef>
#include <type_traits>
#include <vector>
#include <memory>

template<typename T>
std::enable_if_t<std::is_pod_v<T>, size_t> memcpy_from_vector(T *&dest, const std::vector<std::remove_const_t<T>> &vec) {
	dest = static_cast<T*>(malloc(sizeof(T)*vec.size()));
	memcpy(const_cast<std::remove_const_t<T>*>(dest), vec.data(), sizeof(T)*vec.size());
	return vec.size();
}
#endif

#endif