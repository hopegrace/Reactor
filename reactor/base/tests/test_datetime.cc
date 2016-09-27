#include <string>
#include <gtest/gtest.h>
#include "reactor/base/DateTime.h"

using namespace std;
using namespace sduzh::base;

using ::testing::InitGoogleTest;
using ::testing::Test;

TEST(test_DateTime, to_micro_seconds) {
	DateTime t(1000, 1);
	EXPECT_EQ(t.micro_seconds(), 1000 * 1000000 + 1);
}

TEST(test_DateTime, add) {
	DateTime t1(100, 200);
	DateTime t2(100, 700);
	DateTime t3(t1 + t2);
	EXPECT_EQ(t3.micro_seconds(), 200*1000000 + 900);
}

TEST(test_DateTime, add_jinwei) {
	DateTime t1(100, 2);
	DateTime t2(100, 999999);
	DateTime t3(t1 + t2);
	EXPECT_EQ(t3.micro_seconds(), 201*1000000 + 1);
}

TEST(test_DateTime, sub) {
	DateTime t1(100, 200);
	DateTime t2(100, 900);
	DateTime t3(t2 - t1);
	EXPECT_EQ(t3.micro_seconds(), 700);
}

TEST(test_DateTime, sub_jiewei) {
	DateTime t1(102, 200);
	DateTime t2(100, 900);
	DateTime t3(t1 - t2);
	EXPECT_EQ(t3.micro_seconds(), 1000000 + (1000200 - 900));
}

TEST(test_DateTime, self_add) {
	DateTime t1(100, 0);
	DateTime t2(0, 100);

	t1 += t2;
	EXPECT_EQ(t1.micro_seconds(), 100*1000000 + 100);
}

TEST(test_DateTime, self_sub) {
	DateTime t1(100, 0);
	DateTime t2(0, 1);

	t1 -= t2;
	EXPECT_EQ(t1.micro_seconds(), 99*1000000 + 999999);
}

TEST(test_DateTime, compare) {
	DateTime t1(1, 100);
	DateTime t2(2, 10);

	EXPECT_TRUE(t1 == t1);
	EXPECT_TRUE(t1 != t2);
	EXPECT_TRUE(t1 < t2);
	EXPECT_TRUE(t1 <= t2);
	EXPECT_FALSE(t1 > t2);
	EXPECT_FALSE(t1 >= t2);

	EXPECT_TRUE(t2 != t1);
	EXPECT_TRUE(t2 > t1);
	EXPECT_TRUE(t2 >= t1);
	EXPECT_FALSE(t2 < t1);
	EXPECT_FALSE(t2 <= t1);
}

TEST(test_DateTime, to_string) {
	DateTime t1(2016, 10, 1, 12, 34, 56, 123); // 2016-10-01 0:0:0.000
	string s(t1.to_string());
	EXPECT_TRUE(s == "2016-10-01 12:34:56.000123");
}

int main(int argc, char *argv[]) {
	InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
