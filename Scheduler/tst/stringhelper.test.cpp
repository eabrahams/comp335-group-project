#define EXTERN_C
extern "C" {
#include "../src/stringhelper.h"
}
#undef EXTERN_C
#include <gtest/gtest.h>

namespace {

	void regexFreeHelper(regex_info *&regex) {
		pcre2_code_free(regex->re);
		pcre2_match_data_free(regex->match_data);
		free(regex);
	}

	TEST(RegexInit, JobRegex) {
		regex_info *regex = regex_init(JOB_REGEX);
		ASSERT_NE(regex, nullptr);
		regexFreeHelper(regex);
	}

	TEST(StrToJob, InvalidJobPrefix) {
		regex_info *regex = regex_init(JOB_REGEX);
		EXPECT_DEATH(strtojob("this should break", regex), "Bad job string: .*");
		regexFreeHelper(regex);
	}

	TEST(StrToJob, ServerErrorResponse) {
		regex_info *regex = regex_init(JOB_REGEX);
		EXPECT_DEATH(strtojob("ERR", regex), "Bad job string: .*");
		regexFreeHelper(regex);
	}
	
	TEST(StrToJob, AllZeroes) {
		constexpr job_info expected = job_info{ 0, 0, 0, resource_info{0, 0, 0} };
		regex_info *regex = regex_init(JOB_REGEX);
		job_info job = strtojob("JOBN 0 0 0 0 0 0", regex);
		EXPECT_EQ(job.submit_time, expected.submit_time);
		EXPECT_EQ(job.id, expected.id);
		EXPECT_EQ(job.est_runtime, expected.est_runtime);
		EXPECT_EQ(job.req_resc, expected.req_resc);
		regexFreeHelper(regex);
	}

	TEST(StrToJob, StandardExample0) {
		constexpr job_info expected = job_info{ 83, 0, 1566, resource_info{1, 200, 300} };
		regex_info *regex = regex_init(JOB_REGEX);
		job_info job = strtojob("JOBN 83 0 1566 1 200 300", regex);
		EXPECT_EQ(job.submit_time, expected.submit_time);
		EXPECT_EQ(job.id, expected.id);
		EXPECT_EQ(job.est_runtime, expected.est_runtime);
		EXPECT_EQ(job.req_resc, expected.req_resc);
		regexFreeHelper(regex);
	}

	TEST(StrToJob, StandardExample1) {
		constexpr job_info expected = job_info{ 290, 1, 7, resource_info{1, 400, 1200} };
		regex_info *regex = regex_init(JOB_REGEX);
		job_info job = strtojob("JOBN 290 1 7 1 400 1200", regex);
		EXPECT_EQ(job.submit_time, expected.submit_time);
		EXPECT_EQ(job.id, expected.id);
		EXPECT_EQ(job.est_runtime, expected.est_runtime);
		EXPECT_EQ(job.req_resc, expected.req_resc);
		regexFreeHelper(regex);
	}

	TEST(StrToJob, StandardExample2) {
		constexpr job_info expected = job_info{ 336, 2, 96, resource_info{2, 2100, 2800} };
		regex_info *regex = regex_init(JOB_REGEX);
		job_info job = strtojob("JOBN 336 2 96 2 2100 2800", regex);
		EXPECT_EQ(job.submit_time, expected.submit_time);
		EXPECT_EQ(job.id, expected.id);
		EXPECT_EQ(job.est_runtime, expected.est_runtime);
		EXPECT_EQ(job.req_resc, expected.req_resc);
		regexFreeHelper(regex);
	}

	TEST(StrToJob, StandardExample3) {
		constexpr job_info expected = job_info{ 443, 3, 282, resource_info{2, 500, 1100} };
		regex_info *regex = regex_init(JOB_REGEX);
		job_info job = strtojob("JOBN 443 3 282 2 500 1100", regex);
		EXPECT_EQ(job.submit_time, expected.submit_time);
		EXPECT_EQ(job.id, expected.id);
		EXPECT_EQ(job.est_runtime, expected.est_runtime);
		EXPECT_EQ(job.req_resc, expected.req_resc);
		regexFreeHelper(regex);
	}

	TEST(StrToJob, StandardExample4) {
		constexpr job_info expected = job_info{ 728, 4, 1431, resource_info{8, 8000, 15400} };
		regex_info *regex = regex_init(JOB_REGEX);
		job_info job = strtojob("JOBN 728 4 1431 8 8000 15400", regex);
		EXPECT_EQ(job.submit_time, expected.submit_time);
		EXPECT_EQ(job.id, expected.id);
		EXPECT_EQ(job.est_runtime, expected.est_runtime);
		EXPECT_EQ(job.req_resc, expected.req_resc);
		regexFreeHelper(regex);
	}

	TEST(StrToJob, StandardExample5) {
		constexpr job_info expected = job_info{ 867, 5, 133, resource_info{1, 800, 1400} };
		regex_info *regex = regex_init(JOB_REGEX);
		job_info job = strtojob("JOBN 867 5 133 1 800 1400", regex);
		EXPECT_EQ(job.submit_time, expected.submit_time);
		EXPECT_EQ(job.id, expected.id);
		EXPECT_EQ(job.est_runtime, expected.est_runtime);
		EXPECT_EQ(job.req_resc, expected.req_resc);
		regexFreeHelper(regex);
	}

	TEST(StrToJob, StandardExample6) {
		constexpr job_info expected = job_info{ 870, 6, 65, resource_info{1, 900, 200} };
		regex_info *regex = regex_init(JOB_REGEX);
		job_info job = strtojob("JOBN 870 6 65 1 900 200", regex);
		EXPECT_EQ(job.submit_time, expected.submit_time);
		EXPECT_EQ(job.id, expected.id);
		EXPECT_EQ(job.est_runtime, expected.est_runtime);
		EXPECT_EQ(job.req_resc, expected.req_resc);
		regexFreeHelper(regex);
	}

	TEST(StrToJob, StandardExample7) {
		constexpr job_info expected = job_info{ 954, 7, 741, resource_info{1, 1000, 900} };
		regex_info *regex = regex_init(JOB_REGEX);
		job_info job = strtojob("JOBN 954 7 741 1 1000 900", regex);
		EXPECT_EQ(job.submit_time, expected.submit_time);
		EXPECT_EQ(job.id, expected.id);
		EXPECT_EQ(job.est_runtime, expected.est_runtime);
		EXPECT_EQ(job.req_resc, expected.req_resc);
		regexFreeHelper(regex);
	}

	TEST(StrToJob, StandardExample8) {
		constexpr job_info expected = job_info{ 1042, 8, 477, resource_info{1, 800, 1700} };
		regex_info *regex = regex_init(JOB_REGEX);
		job_info job = strtojob("JOBN 1042 8 477 1 800 1700", regex);
		EXPECT_EQ(job.submit_time, expected.submit_time);
		EXPECT_EQ(job.id, expected.id);
		EXPECT_EQ(job.est_runtime, expected.est_runtime);
		EXPECT_EQ(job.req_resc, expected.req_resc);
		regexFreeHelper(regex);
	}

	TEST(StrToJob, StandardExample9) {
		constexpr job_info expected = job_info{ 1064, 9, 47267, resource_info{8, 6300, 4100} };
		regex_info *regex = regex_init(JOB_REGEX);
		job_info job = strtojob("JOBN 1064 9 47267 8 6300 4100", regex);
		EXPECT_EQ(job.submit_time, expected.submit_time);
		EXPECT_EQ(job.id, expected.id);
		EXPECT_EQ(job.est_runtime, expected.est_runtime);
		EXPECT_EQ(job.req_resc, expected.req_resc);
		regexFreeHelper(regex);
	}

}