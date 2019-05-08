#include "../src/job_info.h"
#include <gtest/gtest.h>

namespace {

	TEST(JobFromString, InvalidJobPrefix) {
		EXPECT_DEATH(job_from_string("this should break"), "Bad job string: .*");
	}

	TEST(JobFromString, ServerErrorResponse) {
		EXPECT_DEATH(job_from_string("ERR"), "Bad job string: .*");
	}

	TEST(JobFromString, AllZeroes) {
		constexpr job_info ref_job = job_info{ 0, 0, 0, resource_info{0, 0, 0} };
		job_info str_job = job_from_string("JOBN 0 0 0 0 0 0");
		EXPECT_EQ(str_job.submit_time, ref_job.submit_time);
		EXPECT_EQ(str_job.id, ref_job.id);
		EXPECT_EQ(str_job.est_runtime, ref_job.est_runtime);
		EXPECT_EQ(str_job.req_resc, ref_job.req_resc);
	}
	
	TEST(JobFromString, StandardExample0) {
		constexpr job_info ref_job = job_info{ 83, 0, 1566, resource_info{1, 200, 300} };
		job_info str_job = job_from_string("JOBN 83 0 1566 1 200 300");
		EXPECT_EQ(str_job.submit_time, ref_job.submit_time);
		EXPECT_EQ(str_job.id, ref_job.id);
		EXPECT_EQ(str_job.est_runtime, ref_job.est_runtime);
		EXPECT_EQ(str_job.req_resc, ref_job.req_resc);
	}

	TEST(JobFromString, StandardExample1) {
		constexpr job_info ref_job = job_info{ 290, 1, 7, resource_info{1, 400, 1200} };
		job_info str_job = job_from_string("JOBN 290 1 7 1 400 1200");
		EXPECT_EQ(str_job.submit_time, ref_job.submit_time);
		EXPECT_EQ(str_job.id, ref_job.id);
		EXPECT_EQ(str_job.est_runtime, ref_job.est_runtime);
		EXPECT_EQ(str_job.req_resc, ref_job.req_resc);
	}

	TEST(JobFromString, StandardExample2) {
		constexpr job_info ref_job = job_info{ 336, 2, 96, resource_info{2, 2100, 2800} };
		job_info str_job = job_from_string("JOBN 336 2 96 2 2100 2800");
		EXPECT_EQ(str_job.submit_time, ref_job.submit_time);
		EXPECT_EQ(str_job.id, ref_job.id);
		EXPECT_EQ(str_job.est_runtime, ref_job.est_runtime);
		EXPECT_EQ(str_job.req_resc, ref_job.req_resc);
	}

	TEST(JobFromString, StandardExample3) {
		constexpr job_info ref_job = job_info{ 443, 3, 282, resource_info{2, 500, 1100} };
		job_info str_job = job_from_string("JOBN 443 3 282 2 500 1100");
		EXPECT_EQ(str_job.submit_time, ref_job.submit_time);
		EXPECT_EQ(str_job.id, ref_job.id);
		EXPECT_EQ(str_job.est_runtime, ref_job.est_runtime);
		EXPECT_EQ(str_job.req_resc, ref_job.req_resc);
	}

	TEST(JobFromString, StandardExample4) {
		constexpr job_info ref_job = job_info{ 728, 4, 1431, resource_info{8, 8000, 15400} };
		job_info str_job = job_from_string("JOBN 728 4 1431 8 8000 15400");
		EXPECT_EQ(str_job.submit_time, ref_job.submit_time);
		EXPECT_EQ(str_job.id, ref_job.id);
		EXPECT_EQ(str_job.est_runtime, ref_job.est_runtime);
		EXPECT_EQ(str_job.req_resc, ref_job.req_resc);
	}

	TEST(JobFromString, StandardExample5) {
		constexpr job_info ref_job = job_info{ 867, 5, 133, resource_info{1, 800, 1400} };
		job_info str_job = job_from_string("JOBN 867 5 133 1 800 1400");
		EXPECT_EQ(str_job.submit_time, ref_job.submit_time);
		EXPECT_EQ(str_job.id, ref_job.id);
		EXPECT_EQ(str_job.est_runtime, ref_job.est_runtime);
		EXPECT_EQ(str_job.req_resc, ref_job.req_resc);
	}

	TEST(JobFromString, StandardExample6) {
		constexpr job_info ref_job = job_info{ 870, 6, 65, resource_info{1, 900, 200} };
		job_info str_job = job_from_string("JOBN 870 6 65 1 900 200");
		EXPECT_EQ(str_job.submit_time, ref_job.submit_time);
		EXPECT_EQ(str_job.id, ref_job.id);
		EXPECT_EQ(str_job.est_runtime, ref_job.est_runtime);
		EXPECT_EQ(str_job.req_resc, ref_job.req_resc);
	}

	TEST(JobFromString, StandardExample7) {
		constexpr job_info ref_job = job_info{ 954, 7, 741, resource_info{1, 1000, 900} };
		job_info str_job = job_from_string("JOBN 954 7 741 1 1000 900");
		EXPECT_EQ(str_job.submit_time, ref_job.submit_time);
		EXPECT_EQ(str_job.id, ref_job.id);
		EXPECT_EQ(str_job.est_runtime, ref_job.est_runtime);
		EXPECT_EQ(str_job.req_resc, ref_job.req_resc);
	}

	TEST(JobFromString, StandardExample8) {
		constexpr job_info ref_job = job_info{ 1042, 8, 477, resource_info{1, 800, 1700} };
		job_info str_job = job_from_string("JOBN 1042 8 477 1 800 1700");
		EXPECT_EQ(str_job.submit_time, ref_job.submit_time);
		EXPECT_EQ(str_job.id, ref_job.id);
		EXPECT_EQ(str_job.est_runtime, ref_job.est_runtime);
		EXPECT_EQ(str_job.req_resc, ref_job.req_resc);
	}

	TEST(JobFromString, StandardExample9) {
		constexpr job_info ref_job = job_info{ 1064, 9, 47267, resource_info{8, 6300, 4100} };
		job_info str_job = job_from_string("JOBN 1064 9 47267 8 6300 4100");
		EXPECT_EQ(str_job.submit_time, ref_job.submit_time);
		EXPECT_EQ(str_job.id, ref_job.id);
		EXPECT_EQ(str_job.est_runtime, ref_job.est_runtime);
		EXPECT_EQ(str_job.req_resc, ref_job.req_resc);
	}
}