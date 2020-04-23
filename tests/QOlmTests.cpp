// Spy signal call without having a main loop
#include <QSignalSpy>
#include <QDateTime>
#include <QDebug>

// gtest framework
#include <gtest/gtest.h>

// Our test classes
#include <FooList.hpp>

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
    QSignalSpy spyAboutToInsert(&_list, &QOlm::QOlmBase::rowsAboutToBeInserted);
    QSignalSpy spyInsert(&_list, &QOlm::QOlmBase::rowsInserted);
    ASSERT_EQ(_list.size(), 0);

    auto foo1 = new Foo();
    _list.append(foo1);

    ASSERT_EQ(_list.size(), 1);
    ASSERT_EQ(spyAboutToInsert.count(), 1);
    ASSERT_EQ(spyInsert.count(), 1);

    auto foo2 = new Foo();
    _list.append(foo2);
    _list.append(foo1);

    ASSERT_EQ(_list.size(), 3);
    ASSERT_EQ(spyAboutToInsert.count(), 3);
    ASSERT_EQ(spyInsert.count(), 3);

    ASSERT_TRUE(_list.at(0) == foo1);
    ASSERT_TRUE(_list.at(1) == foo2);
    ASSERT_TRUE(_list.at(2) == foo1);

    _list.clear();

    delete foo1;
    delete foo2;
}

TEST_F(ObjectListTest, AppendFuzz)
{
    auto initTime = QDateTime::currentMSecsSinceEpoch();
    for(int i = 0; i < 10000; ++i)
        _list.append(new Foo());
    auto appendTime = QDateTime::currentMSecsSinceEpoch();
    ASSERT_EQ(_list.size(), 10000);
    int i = 0;
    _list.clear();
    auto clearTime = QDateTime::currentMSecsSinceEpoch();

    qDebug("Append 10000 QObject Time : %llu ms. Clear time : %llu ms", (appendTime - initTime), (clearTime - appendTime));
}