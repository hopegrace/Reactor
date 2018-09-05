#include <unistd.h>
#include <gtest/gtest.h>
#include "reactor/base/path.h"

using namespace std;
using namespace reactor::base;

using ::testing::InitGoogleTest;
using ::testing::Test;

class PathTest: public Test {
public:
	void SetUp() {
		char buff[1024];
		if (::getcwd(buff, sizeof buff)) {
			cwd = buff;
		}
	}

	void TearDown() {
	}

protected:
	std::string cwd;
};

TEST_F(PathTest, abspath) {
	EXPECT_EQ(path::abspath("a"), path::join(cwd, "a"));
	EXPECT_EQ(path::abspath("/a/b"), "/a/b");
	EXPECT_EQ(path::abspath("/a/b/../c"), "/a/c");
	EXPECT_EQ(path::abspath("."), cwd);
}

TEST_F(PathTest, basename) {
	EXPECT_EQ(path::basename("/a/b.c"), "b.c");
	EXPECT_EQ(path::basename(""), "");
	EXPECT_EQ(path::basename("abc"), "abc");
	EXPECT_EQ(path::basename("a/"), "");
}

TEST_F(PathTest, dirname) {
	EXPECT_EQ(path::dirname(""), "");
	EXPECT_EQ(path::dirname("/a/b"), "/a");
	EXPECT_EQ(path::dirname("/a/b/c"), "/a/b");
	EXPECT_EQ(path::dirname("/a/b/c/"), "/a/b/c");
	EXPECT_EQ(path::dirname("b"), "");
	EXPECT_EQ(path::dirname("/a/"), "/a");
	EXPECT_EQ(path::dirname("/"), "/");
}

TEST_F(PathTest, exists) {
	EXPECT_TRUE(path::exists("/tmp"));
	EXPECT_FALSE(path::exists(""));
}

TEST_F(PathTest, isabs) {
	EXPECT_TRUE(path::isabs("/a/b"));
	EXPECT_TRUE(path::isabs("/"));
	EXPECT_FALSE(path::isabs("."));
	EXPECT_FALSE(path::isabs(""));
}

TEST_F(PathTest, isdir) {
	EXPECT_TRUE(path::isdir("/tmp"));
	EXPECT_FALSE(path::isdir("/etc/hosts"));
}

TEST_F(PathTest, isfile) {
	EXPECT_FALSE(path::isfile("/tmp"));
	EXPECT_TRUE(path::isfile("/etc/hosts"));
	EXPECT_FALSE(path::isfile(""));
}

TEST_F(PathTest, ismount) {
	EXPECT_TRUE(path::ismount("/proc"));
	EXPECT_FALSE(path::ismount("/usr"));
}

TEST_F(PathTest, join) {
	EXPECT_EQ(path::join("/a", "b"), "/a/b");
	EXPECT_EQ(path::join("/a", "/b"), "/b");
	EXPECT_EQ(path::join("a", "b"), "a/b");
	EXPECT_EQ(path::join("a/", "b"), "a/b");
	EXPECT_EQ(path::join("", "a"), "a");
	EXPECT_EQ(path::join("", ""), "");
}

TEST_F(PathTest, normpath) {
	EXPECT_EQ(path::normpath("/a"), "/a");
	EXPECT_EQ(path::normpath("/a/"), "/a");
	EXPECT_EQ(path::normpath("/a/."), "/a");
	EXPECT_EQ(path::normpath("/a/./."), "/a");
	EXPECT_EQ(path::normpath("/a/.."), "/");
	EXPECT_EQ(path::normpath("///a/.."), "/");
	EXPECT_EQ(path::normpath("////"), "/");
	EXPECT_EQ(path::normpath("/a///"), "/a");
	EXPECT_EQ(path::normpath("////a/bbb///.././."), "/a");
	EXPECT_EQ(path::normpath("."), ".");
	EXPECT_EQ(path::normpath(".."), "..");
}

TEST_F(PathTest, split) {
	std::pair<string, string> result;

	result = path::split("");
	EXPECT_EQ(result.first, "");
	EXPECT_EQ(result.second, "");

	result = path::split("/ab/cd/ef.txt");
	EXPECT_EQ(result.first, "/ab/cd");
	EXPECT_EQ(result.second, "/ef.txt");

	result = path::split("/ab/cd////ef.txt");
	EXPECT_EQ(result.first, "/ab/cd");
	EXPECT_EQ(result.second, "/ef.txt");

	result = path::split("/txt");
	EXPECT_EQ(result.first, "/");
	EXPECT_EQ(result.second, "/txt");

	result = path::split("txt");
	EXPECT_EQ(result.first, "txt");
	EXPECT_EQ(result.second, "");
}

TEST_F(PathTest, splitext) {
	//EXPECT_EQ(path::splitext("/a/b/c.d"), make_pair("c", ".d"));
}

int main(int argc, char **argv) {
	InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
