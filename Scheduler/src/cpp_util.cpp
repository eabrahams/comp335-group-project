#include "cpp_util.h"

std::string strcpy_and_free(char *src) noexcept {
	std::string str(src);
	free(src);

	return str;
}