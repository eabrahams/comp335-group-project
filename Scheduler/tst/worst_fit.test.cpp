#include "../src/worst_fit.h"
#include <gtest/gtest.h>


namespace {
	/*
	TEST(WorstFitAlgorithm, TrivialCase) {
		constexpr job_info job = job_info{ 0, 0, 0, resource_info{0, 0, 0} };
		system_config *config = parse_config("test-data/defaultconfig-system.xml");
		server_group group = server_group{ &config->servers, config->num_servers };
		server_info *result = worst_fit(config, &group, job);
		EXPECT_STREQ(result->type->name, "4xlarge");
		free_config(config);
		//don't free the group because we didn't allocate anything.
	}
	*/
}