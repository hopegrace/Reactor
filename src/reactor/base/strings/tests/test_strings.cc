#include <string>

#include <gtest/gtest.h>
#include <reactor/base/strings/strings.h>

using namespace std;
using namespace reactor::base::strings;

using ::testing::InitGoogleTest;
using ::testing::Test;

class StringsTest : public Test {
protected:

	string s;
};

TEST_F(StringsTest, lstrip) {
	s = "sduzh";
	lstrip(&s);
	EXPECT_EQ(s, "sduzh");
	EXPECT_EQ(lstrip("sduzh"), "sduzh");

	s = " sduzh";
	lstrip(&s);
	EXPECT_EQ(s, "sduzh");
	EXPECT_EQ(lstrip(" sduzh"), "sduzh");

	s = "\tsduzh";
	lstrip(&s);
	EXPECT_EQ(s, "sduzh");
	EXPECT_EQ(lstrip("\tsduzh"), "sduzh");

	s = "";
	lstrip(&s);
	EXPECT_EQ(s, "");
	EXPECT_EQ(lstrip(""), "");

	s = "   \t";
	lstrip(&s);
	EXPECT_EQ(s, "");
	EXPECT_EQ(lstrip("  \t "), "");
}

TEST_F(StringsTest, rstrip) {
	s = "sduzh";
	rstrip(&s);
	EXPECT_EQ(s, "sduzh");
	EXPECT_EQ(rstrip("sduzh"), "sduzh");

	s = "sduzh \t ";
	rstrip(&s);
	EXPECT_EQ(s, "sduzh");
	EXPECT_EQ(rstrip("sduzh \t"), "sduzh");

	s = "";
	rstrip(&s);
	EXPECT_EQ(s, "");
	EXPECT_EQ(rstrip(""), "");

	s = "   \t";
	rstrip(&s);
	EXPECT_EQ(s, "");
	EXPECT_EQ(rstrip("  \t "), "");
}

TEST_F(StringsTest, strip) {
	s = "sduzh";
	strip(&s);
	EXPECT_EQ(s, "sduzh");
	EXPECT_EQ(strip("sduzh"), "sduzh");

	s = " sduzh \t ";
	strip(&s);
	EXPECT_EQ(s, "sduzh");
	EXPECT_EQ(strip(" sduzh \t"), "sduzh");
	
	s = "";
	strip(&s);
	EXPECT_EQ(s, "");
	EXPECT_EQ(strip(""), "");

	s = "   \t";
	strip(&s);
	EXPECT_EQ(s, "");
	EXPECT_EQ(strip("  \t "), "");
}

TEST_F(StringsTest, to_int) {
	bool ok(false);
	int val(0);

	val = to_int("-1", &ok);
	EXPECT_EQ(val, -1);
	EXPECT_TRUE(ok);

	val = to_int(" -2147483648 ", &ok);
	EXPECT_EQ(val, -2147483648);
	EXPECT_TRUE(ok);

	val = to_int("0", &ok);
	EXPECT_EQ(val, 0);
	EXPECT_TRUE(ok);

	val = to_int("1", &ok);
	EXPECT_EQ(val, 1);
	EXPECT_TRUE(ok);
	
	val = to_int(" 1 ", &ok);
	EXPECT_EQ(val, 1);
	EXPECT_TRUE(ok);

	val = to_int("1", NULL);
	EXPECT_EQ(val, 1);

	val = to_int("2147483647", &ok);
	EXPECT_EQ(val, 2147483647);
	EXPECT_TRUE(ok);

	val = to_int("a1125", &ok);
	EXPECT_FALSE(ok);

	val = to_int("", &ok);
	EXPECT_FALSE(ok);

	val = to_int("0xFF", &ok);
	EXPECT_EQ(val, 255);
	EXPECT_TRUE(ok);
}

TEST_F(StringsTest, to_uint) {
	bool ok(false);
	unsigned int val(0);

	to_uint("-1", &ok);
	EXPECT_FALSE(ok);

	val = to_uint(" 0 ", &ok);
	EXPECT_EQ(val, static_cast<unsigned int>(0));
	EXPECT_TRUE(ok);

	val = to_uint(" 4294967295 ", &ok);
	EXPECT_EQ(val,  4294967295);
	EXPECT_TRUE(ok);
}

TEST_F(StringsTest, to_float) {
	bool ok(false);
	float val(0);

	to_float("-1.12b", &ok);
	EXPECT_FALSE(ok);

	val = to_float(" 0.123 ", &ok);
	EXPECT_FLOAT_EQ(val,static_cast<float>( 0.123));
	EXPECT_TRUE(ok);

	val = to_float("-1.234", &ok);
	EXPECT_FLOAT_EQ(val, static_cast<float>(-1.234));
	EXPECT_TRUE(ok);

	val = to_float("100", &ok);
	EXPECT_FLOAT_EQ(val, static_cast<float>(100));
	EXPECT_TRUE(ok);
}

TEST_F(StringsTest, split) {
	StringList words;
	
	words = split("");
	EXPECT_TRUE(words.empty());

	words = split("  ");
	EXPECT_TRUE(words.empty());

	words = split(" one two  three");
	EXPECT_EQ(words.size(), static_cast<size_t>(3));
	EXPECT_EQ(words[0], "one");
	EXPECT_EQ(words[1], "two");
	EXPECT_EQ(words[2], "three");

	words = split("\none\ntwo\nthree", '\n');
	EXPECT_EQ(words.size(), static_cast<size_t>(3));
	EXPECT_EQ(words[0], "one");
	EXPECT_EQ(words[1], "two");
	EXPECT_EQ(words[2], "three");

	words = split("one two;three", ';');
	EXPECT_EQ(words.size(), static_cast<size_t>(2));
	EXPECT_EQ(words[0], "one two");
	EXPECT_EQ(words[1], "three");
}

TEST_F(StringsTest, startswith) {
	EXPECT_TRUE(startswith("abc", ""));
	EXPECT_TRUE(startswith("abc", "a"));
	EXPECT_TRUE(startswith("abc", "ab"));
	EXPECT_TRUE(startswith("abc", "abc"));
	EXPECT_FALSE(startswith("abc", "abcd"));
	EXPECT_FALSE(startswith("", "a"));
}

TEST_F(StringsTest, endswith) {
	EXPECT_TRUE(endswith("abc", ""));
	EXPECT_TRUE(endswith("abc", "c"));
	EXPECT_TRUE(endswith("abc", "bc"));
	EXPECT_TRUE(endswith("abc", "abc"));
	EXPECT_FALSE(endswith("abc", "abcd"));
	EXPECT_FALSE(endswith("", "a"));
}

int main(int argc, char **argv) {
	InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
