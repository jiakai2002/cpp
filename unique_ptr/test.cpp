#include "gtest/gtest.h"
#include "unique_ptr.hpp"

// Test fixture (optional, not strictly needed here)
class UniquePtrTest : public ::testing::Test {};

// Test construction and dereference
TEST_F(UniquePtrTest, ConstructionAndDereference) {
    unique_ptr<int> a(new int(42));
    EXPECT_TRUE(a);
    EXPECT_EQ(*a, 42);
}

// Test move constructor
TEST_F(UniquePtrTest, MoveConstructor) {
    unique_ptr<int> a(new int(42));
    unique_ptr<int> b(std::move(a));
    EXPECT_FALSE(a);
    EXPECT_EQ(*b, 42);
}

// Test move assignment
TEST_F(UniquePtrTest, MoveAssignment) {
    unique_ptr<int> b(new int(42));
    unique_ptr<int> c;
    c = std::move(b);
    EXPECT_FALSE(b);
    EXPECT_EQ(*c, 42);
}

// Test release
TEST_F(UniquePtrTest, Release) {
    unique_ptr<int> c(new int(42));
    int* raw = c.release();
    EXPECT_FALSE(c);
    EXPECT_EQ(*raw, 42);
    delete raw;
}

// Test reset
TEST_F(UniquePtrTest, Reset) {
    unique_ptr<int> d(new int(100));
    d.reset(new int(200));
    EXPECT_EQ(*d, 200);
}

// Test swap
TEST_F(UniquePtrTest, Swap) {
    unique_ptr<int> e(new int(1));
    unique_ptr<int> f(new int(2));
    e.swap(f);
    EXPECT_EQ(*e, 2);
    EXPECT_EQ(*f, 1);
}

// main function to run all tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}