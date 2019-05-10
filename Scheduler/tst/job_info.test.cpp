#include "../src/job_info.h"
#include <gtest/gtest.h>

namespace {

	TEST(JobCanRun, ExcessiveResources) {
		constexpr job_info job = job_info{ 0, 0, 0, resource_info{1, 2, 3} };
		constexpr auto resources = resource_info{ 4, 4, 4 };
		EXPECT_TRUE(job.can_run(resources));
	}

	TEST(JobCanRun, AdequateResources) {
		constexpr job_info job = job_info{ 0, 0, 0, resource_info{1, 2, 3} };
		constexpr auto resources = resource_info{ 3, 3, 3 };
		EXPECT_TRUE(job.can_run(resources));
	}

	TEST(JobCanRun, ExactResources) {
		constexpr job_info job = job_info{ 0, 0, 0, resource_info{1, 2, 3} };
		constexpr auto resources = resource_info{ 1, 2, 3 };
		EXPECT_TRUE(job.can_run(resources));
	}

	TEST(JobCanRun, InadequateResources) {
		constexpr job_info job = job_info{ 0, 0, 0, resource_info{1, 2, 3} };
		constexpr auto resources = resource_info{ 0, 1, 2 };
		EXPECT_FALSE(job.can_run(resources));
	}

	TEST(JobCanRun, ZeroResources) {
		constexpr job_info job = job_info{ 0, 0, 0, resource_info{1, 2, 3} };
		constexpr auto resources = resource_info{ 0, 0, 0 };
		EXPECT_FALSE(job.can_run(resources));
	}

	TEST(JobCanRun, MismatchResources) {
		constexpr job_info job = job_info{ 0, 0, 0, resource_info{1, 2, 3} };
		constexpr auto resources = resource_info{ 3, 2, 1 };
		EXPECT_FALSE(job.can_run(resources));
	}

	TEST(JobFitness, PositiveFitness) {
		constexpr auto job = job_info{ 0, 0, 0, resource_info {99, 0, 0} };
		constexpr auto resc = resource_info{ 100, 0, 0 };
		EXPECT_EQ(job.fitness(resc), 1);
	}

	TEST(JobFitness, ZeroFitness) {
		constexpr auto job = job_info{ 0, 0, 0, resource_info {100, 0, 0} };
		constexpr auto resc = resource_info{ 100, 0, 0 };
		EXPECT_EQ(job.fitness(resc), 0);
	}

	TEST(JobFitness, NegativeFitness) {
		constexpr auto job = job_info{ 0, 0, 0, resource_info{100, 0, 0} };
		constexpr auto resc = resource_info{ 99, 0, 0 };
		EXPECT_EQ(job.fitness(resc), -1);
	}
}