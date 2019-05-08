#include "../src/resource_info.h"
#include <gtest/gtest.h>

namespace {

	constexpr auto resc_ordered = resource_info{ 0, 1, 2 };
	constexpr auto resc_zero = resource_info{ 0, 0, 0 };
	constexpr auto resc_ones = resource_info{ 1, 1, 1 };
	constexpr auto resc_twos = resource_info{ 2, 2, 2 };
	constexpr auto resc_reverse = resource_info{ 2, 1, 0 };

	TEST(ResourceInfo, Equality) {
		EXPECT_EQ(resc_ordered, resc_ordered);
		EXPECT_EQ(resc_reverse, resc_reverse);
	}

	TEST(ResourceInfo, Inequality) {
		EXPECT_NE(resc_ordered, resc_reverse);
		EXPECT_NE(resc_reverse, resc_ordered);
	}

	TEST(ResourceInfo, LessThan) {
		EXPECT_LT(resc_zero, resc_ordered);
		EXPECT_LT(resc_zero, resc_reverse);
		EXPECT_LT(resc_zero, resc_ones);
		EXPECT_LT(resc_zero, resc_twos);
		EXPECT_LT(resc_ones, resc_twos);
	}

	TEST(ResourceInfo, GreaterThan) {
		EXPECT_GT(resc_ordered, resc_zero);
		EXPECT_GT(resc_reverse, resc_zero);
		EXPECT_GT(resc_ones, resc_zero);
		EXPECT_GT(resc_twos, resc_zero);
		EXPECT_GT(resc_twos, resc_ones);
	}

	TEST(ResourceInfo, LessOrEqual) {
		EXPECT_LE(resc_zero, resc_ordered);
		EXPECT_LE(resc_zero, resc_reverse);
		EXPECT_LE(resc_ordered, resc_twos);
		EXPECT_LE(resc_reverse, resc_twos);
	}
	
	TEST(ResourceInfo, GreaterOrEqual) {
		EXPECT_GE(resc_ordered, resc_zero);
		EXPECT_GE(resc_reverse, resc_zero);
		EXPECT_GE(resc_twos, resc_ordered);
		EXPECT_GE(resc_twos, resc_ordered);
	}

	TEST(ResourceInfo, ComparisonMisc) {
		EXPECT_FALSE(resc_ordered < resc_reverse);
		EXPECT_FALSE(resc_ordered > resc_reverse);
	}

}