#include "gtest/gtest.h"
#include "vector.hpp"

class VectorTest : public ::testing::Test {};

// Test default constructor
TEST_F(VectorTest, DefaultConstructor) {
    vector<int> v1;
    EXPECT_EQ(v1.getSize(), 0);
    EXPECT_EQ(v1.getCapacity(), 0);
}

// Test push_back with copy
TEST_F(VectorTest, PushBackCopy) {
    vector<int> v1;
    v1.push_back(10);
    v1.push_back(20);
    EXPECT_EQ(v1.getSize(), 2);
    EXPECT_EQ(v1[0], 10);
    EXPECT_EQ(v1[1], 20);
}

// Test push_back with move
TEST_F(VectorTest, PushBackMove) {
    vector<int> v1;
    int x = 30;
    v1.push_back(std::move(x));
    EXPECT_EQ(v1.getSize(), 1);
    EXPECT_EQ(v1[0], 30);
}

// Test pop
TEST_F(VectorTest, Pop) {
    vector<int> v1;
    v1.push_back(10);
    v1.push_back(20);
    v1.pop();
    EXPECT_EQ(v1.getSize(), 1);
    EXPECT_EQ(v1[0], 10);
}

// Test copy constructor
TEST_F(VectorTest, CopyConstructor) {
    vector<int> v1;
    v1.push_back(10);
    v1.push_back(20);
    vector<int> v2(v1);
    EXPECT_EQ(v2.getSize(), v1.getSize());
    EXPECT_EQ(v2[0], v1[0]);
    EXPECT_EQ(v2[1], v1[1]);
}

// Test copy assignment
TEST_F(VectorTest, CopyAssignment) {
    vector<int> v1;
    v1.push_back(10);
    v1.push_back(20);
    vector<int> v3;
    v3 = v1;
    EXPECT_EQ(v3.getSize(), v1.getSize());
    EXPECT_EQ(v3[0], v1[0]);
    EXPECT_EQ(v3[1], v1[1]);
}

// Test move constructor
TEST_F(VectorTest, MoveConstructor) {
    vector<int> v1;
    v1.push_back(10);
    v1.push_back(20);
    vector<int> v4(std::move(v1));
    EXPECT_EQ(v4.getSize(), 2);
    EXPECT_EQ(v1.getSize(), 0); // moved-from vector should be empty
}

// Test move assignment
TEST_F(VectorTest, MoveAssignment) {
    vector<int> v1;
    v1.push_back(10);
    v1.push_back(20);
    vector<int> v2(v1);
    vector<int> v5;
    v5 = std::move(v2);
    EXPECT_EQ(v5.getSize(), 2);
    EXPECT_EQ(v2.getSize(), 0); // moved-from vector should be empty
}

// Main function to run all tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}