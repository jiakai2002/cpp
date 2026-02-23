#include "string.hpp"
#include <gtest/gtest.h>

TEST(StringTest, DefaultConstructor) {
    string s;
    EXPECT_TRUE(s.empty());
    EXPECT_EQ(s.getSize(), 0);
    EXPECT_STREQ(s.c_str(), "");
}

TEST(StringTest, CStrConstructor) {
    string s("hello");
    EXPECT_FALSE(s.empty());
    EXPECT_EQ(s.getSize(), 5);
    EXPECT_STREQ(s.c_str(), "hello");
}

TEST(StringTest, CopyConstructor) {
    string s1("copyme");
    string s2(s1);
    EXPECT_EQ(s2.getSize(), s1.getSize());
    EXPECT_STREQ(s2.c_str(), "copyme");
}

TEST(StringTest, MoveConstructor) {
    string s1("move");
    string s2(std::move(s1));
    EXPECT_EQ(s2.getSize(), 4);
    EXPECT_STREQ(s2.c_str(), "move");
    EXPECT_EQ(s1.getSize(), 0);  // moved-from
    EXPECT_STREQ(s1.c_str(), "");
}

TEST(StringTest, CopyAssignment) {
    string s1("abc");
    string s2;
    s2 = s1;
    EXPECT_EQ(s2.getSize(), 3);
    EXPECT_STREQ(s2.c_str(), "abc");
}

TEST(StringTest, MoveAssignment) {
    string s1("xyz");
    string s2;
    s2 = std::move(s1);
    EXPECT_EQ(s2.getSize(), 3);
    EXPECT_STREQ(s2.c_str(), "xyz");
    EXPECT_EQ(s1.getSize(), 0);
    EXPECT_STREQ(s1.c_str(), "");
}

TEST(StringTest, PushBackPopBack) {
    string s("a");
    s.push_back('b');
    EXPECT_EQ(s.getSize(), 2);
    EXPECT_STREQ(s.c_str(), "ab");
    s.pop_back();
    EXPECT_EQ(s.getSize(), 1);
    EXPECT_STREQ(s.c_str(), "a");
}

TEST(StringTest, Append) {
    string s1("foo");
    string s2("bar");
    s1.append(s2);
    EXPECT_EQ(s1.getSize(), 6);
    EXPECT_STREQ(s1.c_str(), "foobar");
}

TEST(StringTest, ShrinkToFit) {
    string s("hello");
    s.reserve(100);
    EXPECT_GE(s.getCapacity(), 100);
    s.shrink_to_fit();
    EXPECT_EQ(s.getCapacity(), s.getSize());
    EXPECT_STREQ(s.c_str(), "hello");
}

TEST(StringTest, Find) {
    string s("abcdef");
    EXPECT_EQ(s.find('c'), 2);
    EXPECT_EQ(s.find('z'), string::npos);

    string t("bcd");
    EXPECT_EQ(s.find(t), 1);
    string u("xyz");
    EXPECT_EQ(s.find(u), string::npos);
}

TEST(StringTest, Substr) {
    string s("hello");
    string sub = s.substr(1,3);
    EXPECT_EQ(sub.getSize(), 3);
    EXPECT_STREQ(sub.c_str(), "ell");
}

TEST(StringTest, Compare) {
    string a("abc");
    string b("abd");
    string c("abc");

    EXPECT_LT(a.compare(b), 0);
    EXPECT_GT(b.compare(a), 0);
    EXPECT_EQ(a.compare(c), 0);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}