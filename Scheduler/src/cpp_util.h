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
#include <string>

// use this to ensure a type is C-safe
#define ASSERT_IS_POD(TYPE) static_assert(std::is_pod< TYPE >::value, __STRING(TYPE) ## " must be POD to be C-safe!")

template<typename T>
typename std::enable_if<std::is_pod<T>::value, size_t>::type memcpy_from_vector(T *&dest, const std::vector<typename std::remove_const<T>::type> &vec) {
	dest = static_cast<T*>(malloc(sizeof(T)*vec.size()));
	memcpy(const_cast<typename std::remove_const<T>::type*>(dest), vec.data(), sizeof(T)*vec.size());
	return vec.size();
}

std::string strcpy_and_free(char *src) noexcept;

#endif

#endif