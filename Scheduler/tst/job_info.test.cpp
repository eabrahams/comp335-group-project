#include "../src/job_info.h"
#include <gtest/gtest.h>

namespace {

	TEST(JobFromString, InvalidJob) {
		EXPECT_DEATH(job_from_string("this should break"), "Bad job string: .*");
	}

	TEST(JobFromString, AllZeroes) {
		constexpr job_info ref_job = job_info{ 0, 0, 0, resource_info{0, 0, 0} };
		job_info str_job = job_from_string("JOBN 0 0 0 0 0 0");
		EXPECT_EQ(str_job.submit_time, ref_job.submit_time);
		EXPECT_EQ(str_job.id, ref_job.id);
		EXPECT_EQ(str_job.est_runtime, ref_job.est_runtime);
		EXPECT_EQ(str_job.req_resc, ref_job.req_resc);
	}

}