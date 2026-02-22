#include "gtest/gtest.h"
#include "shared_ptr.hpp"

struct Foo {
    int x;
    Foo(int val) : x(val) {}
    ~Foo() {}
};

// SharedPtr Tests
TEST(SharedPtrTest, MakeSharedAndUseCount) {
    auto sp1 = make_shared<Foo>(100);
    EXPECT_TRUE(sp1);
    EXPECT_EQ(sp1->x, 100);
    EXPECT_EQ(sp1.use_count(), 1);
}

TEST(SharedPtrTest, CopyConstructorAndAssignment) {
    auto sp1 = make_shared<Foo>(100);

    // copy constructor
    shared_ptr<Foo> sp2 = sp1;
    EXPECT_EQ(sp1.use_count(), 2);
    EXPECT_EQ(sp2.use_count(), 2);

    // move constructor
    shared_ptr<Foo> sp3 = std::move(sp2);
    EXPECT_FALSE(sp2);
    EXPECT_EQ(sp3.use_count(), 2);

    // copy assignment
    shared_ptr<Foo> sp4;
    sp4 = sp3;
    EXPECT_EQ(sp3.use_count(), 3);
    EXPECT_EQ(sp4.use_count(), 3);

    // move assignment
    shared_ptr<Foo> sp5;
    sp5 = std::move(sp4);
    EXPECT_FALSE(sp4);
    EXPECT_EQ(sp5.use_count(), 3);

    // reset shared_ptr
    sp1.reset();
    EXPECT_FALSE(sp1);
    EXPECT_EQ(sp3.use_count(), 2);
}

// WeakPtr Tests
TEST(WeakPtrTest, BasicWeakPtrBehavior) {
    auto sp1 = make_shared<Foo>(100);
    weak_ptr<Foo> wp1 = sp1;

    EXPECT_FALSE(wp1.expired());
    EXPECT_EQ(wp1.use_count(), 1);

    weak_ptr<Foo> wp2 = wp1;
    EXPECT_FALSE(wp2.expired());
    EXPECT_EQ(wp2.use_count(), 1);

    // lock weak_ptr
    auto sp_locked = wp1.lock();
    EXPECT_TRUE(sp_locked);
    EXPECT_EQ(sp_locked->x, 100);
    EXPECT_EQ(sp_locked.use_count(), 2);

    // reset some shared_ptrs
    sp1.reset();
    EXPECT_FALSE(sp1);
    EXPECT_FALSE(wp1.expired());

    sp_locked.reset();
    EXPECT_TRUE(wp1.expired());
    EXPECT_TRUE(wp2.expired());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}