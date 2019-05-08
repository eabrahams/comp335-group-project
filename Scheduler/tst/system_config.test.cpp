#include "../src/system_config.h"
#include <gtest/gtest.h>

namespace {

	TEST(ParseConfig, DefaultConfig) {
		system_config *config = parse_config("test-data/defaultconfig-system.xml");
		ASSERT_NE(config, nullptr);
		EXPECT_EQ(config->num_types, 8);
		EXPECT_EQ(config->num_servers, 184);
		free_config(config);
	}

	TEST(ParseConfig, ConfigSimple1) {
		system_config *config = parse_config("test-data/config_simple1-system.xml");
		ASSERT_NE(config, nullptr);
		EXPECT_EQ(config->num_types, 4);
		EXPECT_EQ(config->num_servers, 80);
		free_config(config);
	}

	TEST(ParseConfig, ConfigSimple2) {
		system_config *config = parse_config("test-data/config_simple2-system.xml");
		ASSERT_NE(config, nullptr);
		EXPECT_EQ(config->num_types, 5);
		EXPECT_EQ(config->num_servers, 100);
		free_config(config);
	}

	TEST(ParseConfig, ConfigSimple5) {
		system_config *config = parse_config("test-data/config_simple5-system.xml");
		ASSERT_NE(config, nullptr);
		EXPECT_EQ(config->num_types, 6);
		EXPECT_EQ(config->num_servers, 120);
		free_config(config);
	}

	TEST(ParseConfig, ConfigSimple6) {
		system_config *config = parse_config("test-data/config_simple6-system.xml");
		ASSERT_NE(config, nullptr);
		EXPECT_EQ(config->num_types, 3);
		EXPECT_EQ(config->num_servers, 6);
		free_config(config);
	}

	TEST(ParseConfig, ExampleConfig) {
		system_config *config = parse_config("test-data/system.xml");
		ASSERT_NE(config, nullptr);
		EXPECT_EQ(config->num_types, 3);
		EXPECT_EQ(config->num_servers, 15);
		free_config(config);
	}
}