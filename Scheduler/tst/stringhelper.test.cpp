#define EXTERN_C
extern "C" {
#include "../src/stringhelper.h"
}
#undef EXTERN_C
#include <gtest/gtest.h>

namespace {

	TEST(RegexInit, JobRegex) {
		regex_info *regex = regex_init(JOB_REGEX);
		ASSERT_NE(regex, nullptr);
		free(regex);
	}

	TEST(StrToJob, InvalidJobPrefix) {
		regex_info *regex = regex_init(JOB_REGEX);
		EXPECT_DEATH(strtojob("this should break", regex), "Bad job string: .*");
		free(regex);
	}

	TEST(StrToJob, ServerErrorResponse) {
		regex_info *regex = regex_init(JOB_REGEX);
		EXPECT_DEATH(strtojob("ERR", regex), "Bad job string: .*");
		free(regex);
	}

}