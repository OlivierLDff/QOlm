// Spy signal call without having a main loop
#include <QSignalSpy>

// gtest framework
#include <gtest/gtest.h>

// Our test classes
#include <FooTest.hpp>

// First basic test fixture that have only one QObject
class ObjectListTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }

    FooList _list;
};

TEST_F(ObjectListTest, Append)
{
    ASSERT_EQ(_list.size(), 0);

    auto foo1 = new Foo();
    _list.append(foo1);

    ASSERT_EQ(_list.size(), 1);

    auto foo2 = new Foo();
    _list.append(foo2);
    _list.append(foo1);

    ASSERT_EQ(_list.size(), 3);

    ASSERT_TRUE(_list.at(0) == foo1);
    ASSERT_TRUE(_list.at(1) == foo2);
    ASSERT_TRUE(_list.at(2) == foo1);

    _list.clear();

    delete foo1;
    delete foo2;
}