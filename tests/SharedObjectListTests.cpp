// Spy signal call without having a main loop
#include <QSignalSpy>
#include <QDateTime>
#include <QDebug>

// gtest framework
#include <gtest/gtest.h>

// Our test classes
#include <SharedFooList.hpp>

// First basic test fixture that have only one QObject
class SharedObjectListTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }

    SharedFooList _list;
};

TEST_F(SharedObjectListTest, Append)
{
    QSignalSpy spyAboutToInsert(&_list, &Olm::SharedObjectListBase::itemAboutToBeInserted);
    QSignalSpy spyInsert(&_list, &Olm::SharedObjectListBase::itemInserted);
    ASSERT_EQ(_list.size(), 0);

    auto foo1 = QSharedPointer<Foo>(new Foo());
    _list.append(foo1);

    ASSERT_EQ(_list.size(), 1);
    ASSERT_EQ(spyAboutToInsert.count(), 1);
    ASSERT_EQ(spyInsert.count(), 1);

    auto foo2 = QSharedPointer<Foo>(new Foo());
    _list.append(foo2);
    _list.append(foo1);

    ASSERT_EQ(_list.size(), 3);
    ASSERT_EQ(spyAboutToInsert.count(), 3);
    ASSERT_EQ(spyInsert.count(), 3);

    ASSERT_TRUE(_list.at(0) == foo1);
    ASSERT_TRUE(_list.at(1) == foo2);
    ASSERT_TRUE(_list.at(2) == foo1);

    _list.clear();
}

TEST_F(SharedObjectListTest, AppendFuzz)
{
    auto initTime = QDateTime::currentMSecsSinceEpoch();
    for(int i = 0; i < 10000; ++i)
        _list.append(QSharedPointer<Foo>(new Foo()));
    auto appendTime = QDateTime::currentMSecsSinceEpoch();
    ASSERT_EQ(_list.size(), 10000);
    _list.clear();
    auto clearTime = QDateTime::currentMSecsSinceEpoch();

    qDebug("Append 10000 Shared QObject Time : %llu ms. Clear time : %llu ms", (appendTime - initTime), (clearTime - appendTime));
}