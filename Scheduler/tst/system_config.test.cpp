#include "../src/system_config.h"
#include <gtest/gtest.h>

namespace {
	constexpr const char* defaultConfigPath = "test-data/defaultconfig-system.xml";
	constexpr const char* configSimple1Path = "test-data/config_simple1-system.xml";
	constexpr const char* configSimple2Path = "test-data/config_simple2-system.xml";
	constexpr const char* configSimple5Path = "test-data/config_simple5-system.xml";
	constexpr const char* configSimple6Path = "test-data/config_simple6-system.xml";
	constexpr const char* exampleConfigPath = "test-data/system.xml";

	TEST(ParseConfig, DefaultConfig) {
		constexpr size_t expected_num_types = 8;
		constexpr const char* expected_names[expected_num_types] = {
			"tiny",
			"small",
			"medium",
			"large",
			"xlarge",
			"2xlarge",
			"3xlarge",
			"4xlarge"
		};
		constexpr unsigned expected_limits[expected_num_types] = {
			7, 30, 30, 30, 20, 20, 40, 7
		};
		constexpr float expected_rates[expected_num_types] = {
			0.1, 0.2, 0.4, 0.8, 1.6, 3.2, 6.4, 12.8
		};
		constexpr resource_info expected_resc[expected_num_types] = {
			resource_info{1, 1000, 4000},
			resource_info{2, 6000, 16000},
			resource_info{4, 20000, 64000},
			resource_info{8, 200000, 256000},
			resource_info{16, 448000, 512000},
			resource_info{32, 288000, 1024000},
			resource_info{64, 512000, 2048000},
			resource_info{128, 1792000, 4096000}
		};

		system_config *config = parse_config(defaultConfigPath);
		ASSERT_NE(config, nullptr);
		ASSERT_EQ(config->num_types, expected_num_types);

		// verify each type, only performed for this particular instance test
		for(auto t = 0; t < expected_num_types; ++t) {
			auto type = config->types[t];
			EXPECT_STREQ(type.name, expected_names[t]) << "With: t=" << t << std::endl;
			EXPECT_EQ(type.limit, expected_limits[t]) << "With: t=" << t << std::endl;
			EXPECT_EQ(type.bootTime, 60);
			EXPECT_EQ(type.rate, expected_rates[t]) << "With: t=" << t << std::endl;
			EXPECT_EQ(type.max_resc, expected_resc[t]) << "With: t=" << t << std::endl;
		}

		EXPECT_EQ(config->num_servers, 184);
		free_config(config);
	}

	TEST(ParseConfig, ConfigSimple1) {
		system_config *config = parse_config(configSimple1Path);
		ASSERT_NE(config, nullptr);
		ASSERT_EQ(config->num_types, 4);
		EXPECT_EQ(config->num_servers, 80);
		free_config(config);
	}

	TEST(ParseConfig, ConfigSimple2) {
		system_config *config = parse_config(configSimple2Path);
		ASSERT_NE(config, nullptr);
		ASSERT_EQ(config->num_types, 5);
		EXPECT_EQ(config->num_servers, 100);
		free_config(config);
	}

	TEST(ParseConfig, ConfigSimple5) {
		system_config *config = parse_config(configSimple5Path);
		ASSERT_NE(config, nullptr);
		ASSERT_EQ(config->num_types, 6);
		EXPECT_EQ(config->num_servers, 120);
		free_config(config);
	}

	TEST(ParseConfig, ConfigSimple6) {
		system_config *config = parse_config(configSimple6Path);
		ASSERT_NE(config, nullptr);
		ASSERT_EQ(config->num_types, 3);
		EXPECT_EQ(config->num_servers, 6);
		free_config(config);
	}

	TEST(ParseConfig, ExampleConfig) {
		system_config *config = parse_config(exampleConfigPath);
		ASSERT_NE(config, nullptr);
		ASSERT_EQ(config->num_types, 3);
		EXPECT_EQ(config->num_servers, 15);
		free_config(config);
	}

	TEST(TypeByName, ExistingTypes) {
		system_config *config = parse_config(defaultConfigPath);
		auto *type = &config->types[0];
		for(auto t = 0; t < config->num_types; ++t) {
			auto *type = &config->types[0];
			EXPECT_EQ(config->type_by_name(type->name), type) << "With name=" << type->name << std::endl;
		}
		free_config(config);
	}

	TEST(TypeByName, NoSuchType) {
		system_config *config = parse_config(exampleConfigPath);
		EXPECT_THROW(config->type_by_name("this should break"), std::invalid_argument);
		free_config(config);
	}
}