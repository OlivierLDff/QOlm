// Our test classes
#include <QOlm/QOlm.hpp>

// gtest framework
#include <gtest/gtest.h>
#include <gmock/gmock.h>

// Spy signal call without having a main loop
#include <QSignalSpy>

// stl headers
#include <queue>
#include <utility>

class QOlmTestListFilled : public ::testing::Test
{
protected:
    void SetUp() override { list.append({foo1, foo2, foo3, foo4}); }
    void TearDown() override {}

    qolm::QOlm<QObject> list;
    QObject* foo1 = new QObject(&list);
    QObject* foo2 = new QObject(&list);
    QObject* foo3 = new QObject(&list);
    QObject* foo4 = new QObject(&list);
    QObject* foo5 = new QObject(&list);
};

TEST_F(QOlmTestListFilled, At)
{
    ASSERT_EQ(list.at(2), foo3);
    ASSERT_EQ(list.at(4), nullptr);
    ASSERT_EQ(list.at(-1), nullptr);
}

TEST_F(QOlmTestListFilled, Get)
{
    ASSERT_EQ(list.get(1), foo2);
    ASSERT_EQ(list.get(4), nullptr);
    ASSERT_EQ(list.get(-1), nullptr);
}

TEST_F(QOlmTestListFilled, Contains)
{
    ASSERT_TRUE(list.contains(foo1));
    ASSERT_TRUE(list.contains(foo2));
    ASSERT_TRUE(list.contains(foo3));
    ASSERT_TRUE(list.contains(foo4));

    //With a null pointer
    ASSERT_FALSE(list.contains(foo5));
    ASSERT_FALSE(list.contains(nullptr));
}

TEST_F(QOlmTestListFilled, IndexOf)
{
    ASSERT_EQ(list.indexOf(foo1), 0);
    ASSERT_EQ(list.indexOf(foo2), 1);
    ASSERT_EQ(list.indexOf(foo3), 2);

    //With a null pointer
    ASSERT_EQ(list.indexOf(nullptr), -1);

    //With an object not in the list
    ASSERT_EQ(list.indexOf(foo5), -1);
}

TEST_F(QOlmTestListFilled, Count) { ASSERT_EQ(list.count(), 4); }

template<class T = QObject>
class QOlmMock : public qolm::QOlm<T>
{
public:
    MOCK_METHOD(void, onObjectAboutToBeInserted, (T * object, int index), (override));
    MOCK_METHOD(void, onObjectInserted, (T * object, int index), (override));
    MOCK_METHOD(void, onObjectAboutToBeMoved, (T * object, int src, int dest), (override));
    MOCK_METHOD(void, onObjectMoved, (T * object, int src, int dest), (override));
    MOCK_METHOD(void, onObjectAboutToBeRemoved, (T * object, int index), (override));
    MOCK_METHOD(void, onObjectRemoved, (T * object, int index), (override));
};

// First basic test fixture that have only one QObject
class QOlmTestMock : public ::testing::Test
{
protected:
    QOlmMock<QObject> list;

    QObject* foo1 = new QObject(&list);
    QObject* foo2 = new QObject(&list);
    QObject* foo3 = new QObject(&list);
    QObject* foo4 = new QObject(&list);
    QObject* foo5 = new QObject(&list);
};

TEST_F(QOlmTestMock, AppendQObject)
{
    QSignalSpy spyAboutToInsert(&list, &qolm::QOlmBase::rowsAboutToBeInserted);
    QSignalSpy spyInsert(&list, &qolm::QOlmBase::rowsInserted);

    EXPECT_CALL(list, onObjectAboutToBeInserted(foo1, 0));
    EXPECT_CALL(list, onObjectInserted(foo1, 0));
    EXPECT_CALL(list, onObjectAboutToBeInserted(foo2, 1));
    EXPECT_CALL(list, onObjectInserted(foo2, 1));
    EXPECT_CALL(list, onObjectAboutToBeInserted(foo1, 2));
    EXPECT_CALL(list, onObjectInserted(foo1, 2));

    ASSERT_EQ(list.size(), 0);

    bool expectedCallback = false;
    int expectedIndex = -1;
    QObject* expectedObject = nullptr;

    list.onInserted(
        [&](QObject* object, int index)
        {
            ASSERT_TRUE(expectedCallback);
            ASSERT_EQ(expectedIndex, index);
            ASSERT_EQ(expectedObject, object);

            expectedCallback = false;
            expectedIndex = -1;
            expectedObject = nullptr;
        });
    list.onMoved([&](QObject* object, int from, int to) { ASSERT_TRUE(false); });
    list.onRemoved([&](QObject* object, int index) { ASSERT_TRUE(false); });

    expectedCallback = true;
    expectedIndex = 0;
    expectedObject = foo1;
    list.append(foo1);

    ASSERT_EQ(list.size(), 1);
    EXPECT_FALSE(expectedCallback);
    ASSERT_EQ(spyAboutToInsert.count(), 1);
    ASSERT_EQ(spyInsert.count(), 1);
    {
        const auto args = spyAboutToInsert.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 0);
        ASSERT_EQ(last, 0);
    }
    {
        const auto args = spyInsert.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 0);
        ASSERT_EQ(last, 0);
    }

    expectedCallback = true;
    expectedIndex = 1;
    expectedObject = foo2;
    list.append(foo2);

    expectedCallback = true;
    expectedIndex = 2;
    expectedObject = foo1;
    list.append(foo1);

    EXPECT_FALSE(expectedCallback);
    ASSERT_EQ(list.size(), 3);
    ASSERT_EQ(spyAboutToInsert.count(), 2);
    ASSERT_EQ(spyInsert.count(), 2);
    {
        const auto args = spyAboutToInsert.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 1);
        ASSERT_EQ(last, 1);
    }
    {
        const auto args = spyAboutToInsert.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 2);
        ASSERT_EQ(last, 2);
    }
    {
        const auto args = spyInsert.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 1);
        ASSERT_EQ(last, 1);
    }
    {
        const auto args = spyInsert.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 2);
        ASSERT_EQ(last, 2);
    }

    ASSERT_EQ(list.get(0), foo1);
    ASSERT_EQ(list.get(1), foo2);
    ASSERT_EQ(list.get(2), foo1);

    //Append a null pointer Object. Not signal should be called
    list.append(nullptr);

    ASSERT_EQ(list.size(), 3);

    ASSERT_EQ(spyAboutToInsert.count(), 0);
    ASSERT_EQ(spyInsert.count(), 0);

    // Check list is untouched
    ASSERT_EQ(list.get(0), foo1);
    ASSERT_EQ(list.get(1), foo2);
    ASSERT_EQ(list.get(2), foo1);
}

TEST_F(QOlmTestMock, PrependQObject)
{
    QSignalSpy spyAboutToInsert(&list, &qolm::QOlmBase::rowsAboutToBeInserted);
    QSignalSpy spyInsert(&list, &qolm::QOlmBase::rowsInserted);

    EXPECT_CALL(list, onObjectAboutToBeInserted(foo1, 0));
    EXPECT_CALL(list, onObjectInserted(foo1, 0));
    EXPECT_CALL(list, onObjectAboutToBeInserted(foo2, 0));
    EXPECT_CALL(list, onObjectInserted(foo2, 0));

    ASSERT_EQ(list.size(), 0);

    bool expectedCallback = false;
    int expectedIndex = -1;
    QObject* expectedObject = nullptr;

    list.onInserted(
        [&](QObject* object, int index)
        {
            ASSERT_TRUE(expectedCallback);
            ASSERT_EQ(expectedIndex, index);
            ASSERT_EQ(expectedObject, object);

            expectedCallback = false;
            expectedIndex = -1;
            expectedObject = nullptr;
        });
    list.onMoved([&](QObject* object, int from, int to) { ASSERT_TRUE(false); });
    list.onRemoved([&](QObject* object, int index) { ASSERT_TRUE(false); });

    expectedCallback = true;
    expectedIndex = 0;
    expectedObject = foo1;

    list.prepend(foo1);

    EXPECT_FALSE(expectedCallback);
    ASSERT_EQ(list.size(), 1);
    ASSERT_EQ(spyAboutToInsert.count(), 1);
    ASSERT_EQ(spyInsert.count(), 1);
    {
        const auto args = spyAboutToInsert.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 0);
        ASSERT_EQ(last, 0);
    }
    {
        const auto args = spyInsert.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 0);
        ASSERT_EQ(last, 0);
    }
    ASSERT_EQ(list.get(0), foo1);

    expectedCallback = true;
    expectedIndex = 0;
    expectedObject = foo2;

    list.prepend(foo2);

    EXPECT_FALSE(expectedCallback);
    ASSERT_EQ(list.size(), 2);
    ASSERT_EQ(spyAboutToInsert.count(), 1);
    ASSERT_EQ(spyInsert.count(), 1);
    {
        const auto args = spyAboutToInsert.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 0);
        ASSERT_EQ(last, 0);
    }
    {
        const auto args = spyInsert.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 0);
        ASSERT_EQ(last, 0);
    }

    ASSERT_EQ(list.get(0), foo2);
    ASSERT_EQ(list.get(1), foo1);

    //Prepend a null pointer Object
    list.prepend(nullptr);

    ASSERT_EQ(list.size(), 2);
    ASSERT_EQ(spyAboutToInsert.count(), 0);
    ASSERT_EQ(spyInsert.count(), 0);

    ASSERT_EQ(list.get(0), foo2);
    ASSERT_EQ(list.get(1), foo1);
}

TEST_F(QOlmTestMock, InsertQObject)
{
    QSignalSpy spyAboutToInsert(&list, &qolm::QOlmBase::rowsAboutToBeInserted);
    QSignalSpy spyInsert(&list, &qolm::QOlmBase::rowsInserted);

    EXPECT_CALL(list, onObjectAboutToBeInserted(foo1, 0));
    EXPECT_CALL(list, onObjectInserted(foo1, 0));
    EXPECT_CALL(list, onObjectAboutToBeInserted(foo2, 0));
    EXPECT_CALL(list, onObjectInserted(foo2, 0));
    EXPECT_CALL(list, onObjectAboutToBeInserted(foo2, 2));
    EXPECT_CALL(list, onObjectInserted(foo2, 2));
    EXPECT_CALL(list, onObjectAboutToBeInserted(foo2, 1));
    EXPECT_CALL(list, onObjectInserted(foo2, 1));

    ASSERT_EQ(list.size(), 0);

    bool expectedCallback = false;
    int expectedIndex = -1;
    QObject* expectedObject = nullptr;

    list.onInserted(
        [&](QObject* object, int index)
        {
            ASSERT_TRUE(expectedCallback);
            ASSERT_EQ(expectedIndex, index);
            ASSERT_EQ(expectedObject, object);

            expectedCallback = false;
            expectedIndex = -1;
            expectedObject = nullptr;
        });
    list.onMoved([&](QObject* object, int from, int to) { ASSERT_TRUE(false); });
    list.onRemoved([&](QObject* object, int index) { ASSERT_TRUE(false); });

    expectedCallback = true;
    expectedIndex = 0;
    expectedObject = foo1;

    list.insert(0, foo1);

    EXPECT_FALSE(expectedCallback);
    ASSERT_EQ(list.size(), 1);
    ASSERT_EQ(spyAboutToInsert.count(), 1);
    ASSERT_EQ(spyInsert.count(), 1);
    {
        const auto args = spyAboutToInsert.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 0);
        ASSERT_EQ(last, 0);
    }
    {
        const auto args = spyInsert.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 0);
        ASSERT_EQ(last, 0);
    }

    expectedCallback = true;
    expectedIndex = 0;
    expectedObject = foo2;

    list.insert(0, foo2);

    EXPECT_FALSE(expectedCallback);
    ASSERT_EQ(list.size(), 2);
    ASSERT_EQ(spyAboutToInsert.count(), 1);
    ASSERT_EQ(spyInsert.count(), 1);
    {
        const auto args = spyAboutToInsert.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 0);
        ASSERT_EQ(last, 0);
    }
    {
        const auto args = spyInsert.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 0);
        ASSERT_EQ(last, 0);
    }

    ASSERT_EQ(list.get(0), foo2);
    ASSERT_EQ(list.get(1), foo1);

    // if the user put an index greater than the maximal index in the list, the object will be inserted get the end of the list

    expectedCallback = true;
    expectedIndex = 2;
    expectedObject = foo2;

    list.insert(4, foo2);

    EXPECT_FALSE(expectedCallback);
    ASSERT_EQ(list.size(), 3);
    ASSERT_EQ(spyAboutToInsert.count(), 1);
    ASSERT_EQ(spyInsert.count(), 1);
    {
        const auto args = spyAboutToInsert.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 2);
        ASSERT_EQ(last, 2);
    }
    {
        const auto args = spyInsert.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 2);
        ASSERT_EQ(last, 2);
    }

    ASSERT_EQ(list.get(0), foo2);
    ASSERT_EQ(list.get(1), foo1);
    ASSERT_EQ(list.get(2), foo2);

    //if the user insert an object get an index which already contains an object, it will create a shift in the list with the next objects

    expectedCallback = true;
    expectedIndex = 1;
    expectedObject = foo2;

    list.insert(1, foo2);

    EXPECT_FALSE(expectedCallback);
    ASSERT_EQ(list.size(), 4);
    ASSERT_EQ(spyAboutToInsert.count(), 1);
    ASSERT_EQ(spyInsert.count(), 1);
    {
        const auto args = spyAboutToInsert.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 1);
        ASSERT_EQ(last, 1);
    }
    {
        const auto args = spyInsert.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 1);
        ASSERT_EQ(last, 1);
    }

    ASSERT_EQ(list.get(0), foo2);
    ASSERT_EQ(list.get(1), foo2);
    ASSERT_EQ(list.get(2), foo1);
    ASSERT_EQ(list.get(3), foo2);

    //Null pointer insert

    list.insert(1, nullptr);

    EXPECT_FALSE(expectedCallback);
    ASSERT_EQ(list.size(), 4);
    ASSERT_EQ(spyAboutToInsert.count(), 0);
    ASSERT_EQ(spyInsert.count(), 0);
}

TEST_F(QOlmTestMock, InsertList)
{
    QSignalSpy spyAboutToInsert(&list, &qolm::QOlmBase::rowsAboutToBeInserted);
    QSignalSpy spyInsert(&list, &qolm::QOlmBase::rowsInserted);

    EXPECT_CALL(list, onObjectAboutToBeInserted(foo1, 0));
    EXPECT_CALL(list, onObjectInserted(foo1, 0));
    EXPECT_CALL(list, onObjectAboutToBeInserted(foo2, 1));
    EXPECT_CALL(list, onObjectInserted(foo2, 1));
    EXPECT_CALL(list, onObjectAboutToBeInserted(foo3, 2));
    EXPECT_CALL(list, onObjectInserted(foo3, 2));
    EXPECT_CALL(list, onObjectAboutToBeInserted(foo1, 1));
    EXPECT_CALL(list, onObjectInserted(foo1, 1));
    EXPECT_CALL(list, onObjectAboutToBeInserted(foo2, 2));
    EXPECT_CALL(list, onObjectInserted(foo2, 2));
    EXPECT_CALL(list, onObjectAboutToBeInserted(foo3, 3));
    EXPECT_CALL(list, onObjectInserted(foo3, 3));
    ASSERT_EQ(list.size(), 0);

    std::queue<std::pair<int, QObject*>> expectedCallback;

    list.onInserted(
        [&](QObject* object, int index)
        {
            ASSERT_TRUE(!expectedCallback.empty());
            const auto [expectedIndex, expectedObject] = expectedCallback.front();
            ASSERT_EQ(index, expectedIndex);
            ASSERT_EQ(object, expectedObject);

            expectedCallback.pop();
        });
    list.onMoved([&](QObject* object, int from, int to) { ASSERT_TRUE(false); });
    list.onRemoved([&](QObject* object, int index) { ASSERT_TRUE(false); });

    expectedCallback.push(std::make_pair(0, foo1));
    expectedCallback.push(std::make_pair(1, foo2));
    expectedCallback.push(std::make_pair(2, foo3));
    list.insert(0, {foo1, foo2, foo3});

    EXPECT_TRUE(expectedCallback.empty());
    ASSERT_EQ(list.size(), 3);
    ASSERT_EQ(spyAboutToInsert.count(), 1);
    ASSERT_EQ(spyInsert.count(), 1);
    {
        const auto args = spyAboutToInsert.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 0);
        ASSERT_EQ(last, 2);
    }
    {
        const auto args = spyInsert.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 0);
        ASSERT_EQ(last, 2);
    }

    ASSERT_EQ(list.get(0), foo1);
    ASSERT_EQ(list.get(1), foo2);
    ASSERT_EQ(list.get(2), foo3);

    expectedCallback.push(std::make_pair(1, foo1));
    expectedCallback.push(std::make_pair(2, foo2));
    expectedCallback.push(std::make_pair(3, foo3));
    list.insert(1, {foo1, foo2, foo3});

    EXPECT_TRUE(expectedCallback.empty());
    ASSERT_EQ(list.size(), 6);
    ASSERT_EQ(spyAboutToInsert.count(), 1);
    ASSERT_EQ(spyInsert.count(), 1);
    {
        const auto args = spyAboutToInsert.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 1);
        ASSERT_EQ(last, 3);
    }
    {
        const auto args = spyInsert.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 1);
        ASSERT_EQ(last, 3);
    }

    ASSERT_EQ(list.get(0), foo1);
    ASSERT_EQ(list.get(1), foo1);
    ASSERT_EQ(list.get(2), foo2);
    ASSERT_EQ(list.get(3), foo3);
    ASSERT_EQ(list.get(4), foo2);
    ASSERT_EQ(list.get(5), foo3);

    //Empty List
    list.insert(2, QList<QObject*>());
    ASSERT_EQ(list.size(), 6);
    ASSERT_EQ(spyAboutToInsert.count(), 0);
    ASSERT_EQ(spyInsert.count(), 0);

    ASSERT_EQ(list.get(0), foo1);
    ASSERT_EQ(list.get(1), foo1);
    ASSERT_EQ(list.get(2), foo2);
    ASSERT_EQ(list.get(3), foo3);
    ASSERT_EQ(list.get(4), foo2);
    ASSERT_EQ(list.get(5), foo3);
}

TEST_F(QOlmTestMock, AppendList)
{
    QSignalSpy spyAboutToInsert(&list, &qolm::QOlmBase::rowsAboutToBeInserted);
    QSignalSpy spyInsert(&list, &qolm::QOlmBase::rowsInserted);

    EXPECT_CALL(list, onObjectAboutToBeInserted(foo1, 0));
    EXPECT_CALL(list, onObjectInserted(foo1, 0));
    EXPECT_CALL(list, onObjectAboutToBeInserted(foo2, 1));
    EXPECT_CALL(list, onObjectInserted(foo2, 1));
    EXPECT_CALL(list, onObjectAboutToBeInserted(foo3, 2));
    EXPECT_CALL(list, onObjectInserted(foo3, 2));
    EXPECT_CALL(list, onObjectAboutToBeInserted(foo2, 3));
    EXPECT_CALL(list, onObjectInserted(foo2, 3));
    EXPECT_CALL(list, onObjectAboutToBeInserted(foo1, 4));
    EXPECT_CALL(list, onObjectInserted(foo1, 4));
    EXPECT_CALL(list, onObjectAboutToBeInserted(foo3, 5));
    EXPECT_CALL(list, onObjectInserted(foo3, 5));

    ASSERT_EQ(list.size(), 0);

    std::queue<std::pair<int, QObject*>> expectedCallback;

    list.onInserted(
        [&](QObject* object, int index)
        {
            ASSERT_TRUE(!expectedCallback.empty());
            const auto [expectedIndex, expectedObject] = expectedCallback.front();
            ASSERT_EQ(expectedIndex, index);
            ASSERT_EQ(expectedObject, object);

            expectedCallback.pop();
        });

    expectedCallback.push(std::make_pair(0, foo1));
    expectedCallback.push(std::make_pair(1, foo2));
    expectedCallback.push(std::make_pair(2, foo3));
    list.append({foo1, foo2, foo3});

    EXPECT_TRUE(expectedCallback.empty());
    ASSERT_EQ(list.size(), 3);
    ASSERT_EQ(spyAboutToInsert.count(), 1);
    ASSERT_EQ(spyInsert.count(), 1);
    {
        const auto args = spyAboutToInsert.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 0);
        ASSERT_EQ(last, 2);
    }
    {
        const auto args = spyInsert.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 0);
        ASSERT_EQ(last, 2);
    }

    ASSERT_EQ(list.get(0), foo1);
    ASSERT_EQ(list.get(1), foo2);
    ASSERT_EQ(list.get(2), foo3);

    expectedCallback.push(std::make_pair(3, foo2));
    expectedCallback.push(std::make_pair(4, foo1));
    expectedCallback.push(std::make_pair(5, foo3));
    list.append({foo2, foo1, foo3});

    EXPECT_TRUE(expectedCallback.empty());
    ASSERT_EQ(list.size(), 6);
    ASSERT_EQ(spyAboutToInsert.count(), 1);
    ASSERT_EQ(spyInsert.count(), 1);
    {
        const auto args = spyAboutToInsert.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 3);
        ASSERT_EQ(last, 5);
    }
    {
        const auto args = spyInsert.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 3);
        ASSERT_EQ(last, 5);
    }

    ASSERT_EQ(list.get(0), foo1);
    ASSERT_EQ(list.get(1), foo2);
    ASSERT_EQ(list.get(2), foo3);
    ASSERT_EQ(list.get(3), foo2);
    ASSERT_EQ(list.get(4), foo1);
    ASSERT_EQ(list.get(5), foo3);

    //Empty List
    list.append(QList<QObject*>());
    ASSERT_EQ(list.size(), 6);
    ASSERT_EQ(spyAboutToInsert.count(), 0);
    ASSERT_EQ(spyInsert.count(), 0);
}

TEST_F(QOlmTestMock, PrependList)
{
    QSignalSpy spyAboutToInsert(&list, &qolm::QOlmBase::rowsAboutToBeInserted);
    QSignalSpy spyInsert(&list, &qolm::QOlmBase::rowsInserted);

    EXPECT_CALL(list, onObjectAboutToBeInserted(foo1, 0));
    EXPECT_CALL(list, onObjectInserted(foo1, 0));
    EXPECT_CALL(list, onObjectAboutToBeInserted(foo2, 1));
    EXPECT_CALL(list, onObjectInserted(foo2, 1));
    EXPECT_CALL(list, onObjectAboutToBeInserted(foo3, 2)).Times(2);
    EXPECT_CALL(list, onObjectInserted(foo3, 2)).Times(2);
    EXPECT_CALL(list, onObjectAboutToBeInserted(foo2, 0));
    EXPECT_CALL(list, onObjectInserted(foo2, 0));
    EXPECT_CALL(list, onObjectAboutToBeInserted(foo1, 1));
    EXPECT_CALL(list, onObjectInserted(foo1, 1));
    ASSERT_EQ(list.size(), 0);

    std::queue<std::pair<int, QObject*>> expectedCallback;

    list.onInserted(
        [&](QObject* object, int index)
        {
            ASSERT_TRUE(!expectedCallback.empty());
            const auto [expectedIndex, expectedObject] = expectedCallback.front();
            ASSERT_EQ(expectedIndex, index);
            ASSERT_EQ(expectedObject, object);

            expectedCallback.pop();
        });

    expectedCallback.push(std::make_pair(0, foo1));
    expectedCallback.push(std::make_pair(1, foo2));
    expectedCallback.push(std::make_pair(2, foo3));
    list.prepend({foo1, foo2, foo3});

    EXPECT_TRUE(expectedCallback.empty());
    ASSERT_EQ(list.size(), 3);
    ASSERT_EQ(spyAboutToInsert.count(), 1);
    ASSERT_EQ(spyInsert.count(), 1);
    {
        const auto args = spyAboutToInsert.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 0);
        ASSERT_EQ(last, 2);
    }
    {
        const auto args = spyInsert.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 0);
        ASSERT_EQ(last, 2);
    }

    ASSERT_EQ(list.get(0), foo1);
    ASSERT_EQ(list.get(1), foo2);
    ASSERT_EQ(list.get(2), foo3);

    expectedCallback.push(std::make_pair(0, foo2));
    expectedCallback.push(std::make_pair(1, foo1));
    expectedCallback.push(std::make_pair(2, foo3));
    list.prepend({foo2, foo1, foo3});

    EXPECT_TRUE(expectedCallback.empty());
    ASSERT_EQ(list.size(), 6);
    ASSERT_EQ(spyAboutToInsert.count(), 1);
    ASSERT_EQ(spyInsert.count(), 1);
    {
        const auto args = spyAboutToInsert.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 0);
        ASSERT_EQ(last, 2);
    }
    {
        const auto args = spyInsert.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 0);
        ASSERT_EQ(last, 2);
    }

    ASSERT_EQ(list.get(0), foo2);
    ASSERT_EQ(list.get(1), foo1);
    ASSERT_EQ(list.get(2), foo3);
    ASSERT_EQ(list.get(3), foo1);
    ASSERT_EQ(list.get(4), foo2);
    ASSERT_EQ(list.get(5), foo3);

    //Empty List
    list.prepend(QList<QObject*>());
    ASSERT_EQ(list.size(), 6);
    ASSERT_EQ(spyAboutToInsert.count(), 0);
    ASSERT_EQ(spyInsert.count(), 0);
}

class QOlmTestMockListFilled : public ::testing::Test
{
protected:
    void SetUp() override { list.append({foo1, foo2, foo3, foo4}); }
    void TearDown() override {}

    ::testing::NiceMock<QOlmMock<QObject>> list;
    QObject* foo1 = new QObject(&list);
    QObject* foo2 = new QObject(&list);
    QObject* foo3 = new QObject(&list);
    QObject* foo4 = new QObject(&list);
    QObject* foo5 = new QObject(&list);
};

TEST_F(QOlmTestMockListFilled, Move)
{
    QSignalSpy spyAboutToMoved(&list, &qolm::QOlmBase::rowsAboutToBeMoved);
    QSignalSpy spyMoved(&list, &qolm::QOlmBase::rowsMoved);

    EXPECT_CALL(list, onObjectAboutToBeMoved(foo1, 0, 2));
    EXPECT_CALL(list, onObjectMoved(foo1, 0, 2));
    EXPECT_CALL(list, onObjectAboutToBeMoved(foo2, 0, 3));
    EXPECT_CALL(list, onObjectMoved(foo2, 0, 3));

    ASSERT_EQ(list.size(), 4);

    bool expectedCallback = false;
    int expectedIndexFrom = -1;
    int expectedIndexTo = -1;
    QObject* expectedObject = nullptr;
    list.onMoved(
        [&](QObject* object, int from, int to)
        {
            ASSERT_TRUE(expectedCallback);
            ASSERT_EQ(expectedIndexFrom, from);
            ASSERT_EQ(expectedIndexTo, to);
            ASSERT_EQ(expectedObject, object);

            expectedCallback = false;
            expectedIndexFrom = -1;
            expectedIndexTo = -1;
            expectedObject = nullptr;
        });

    expectedCallback = true;
    expectedIndexFrom = 0;
    expectedIndexTo = 2;
    expectedObject = foo1;

    list.move(0, 2);
    EXPECT_FALSE(expectedCallback);
    ASSERT_EQ(spyAboutToMoved.count(), 1);
    ASSERT_EQ(spyMoved.count(), 1);
    {
        const auto args = spyAboutToMoved.takeFirst();
        const auto start = args.at(1).toInt();
        const auto end = args.at(2).toInt();
        const auto row = args.at(4).toInt();
        ASSERT_EQ(start, 0);
        ASSERT_EQ(end, 0);
        ASSERT_EQ(row, 3);
    }
    {
        const auto args = spyMoved.takeFirst();
        const auto start = args.at(1).toInt();
        const auto end = args.at(2).toInt();
        const auto row = args.at(4).toInt();
        ASSERT_EQ(start, 0);
        ASSERT_EQ(end, 0);
        ASSERT_EQ(row, 3);
    }

    ASSERT_EQ(list.get(0), foo2);
    ASSERT_EQ(list.get(1), foo3);
    ASSERT_EQ(list.get(2), foo1);
    ASSERT_EQ(list.get(3), foo4);

    //Warnings expected

    expectedCallback = true;
    expectedIndexFrom = 0;
    expectedIndexTo = 3;
    expectedObject = foo2;

    list.move(0, 5);
    EXPECT_FALSE(expectedCallback);
    ASSERT_EQ(spyAboutToMoved.count(), 1);
    ASSERT_EQ(spyMoved.count(), 1);
    {
        const auto args = spyAboutToMoved.takeFirst();
        const auto start = args.at(1).toInt();
        const auto end = args.at(2).toInt();
        const auto row = args.at(4).toInt();
        ASSERT_EQ(start, 0);
        ASSERT_EQ(end, 0);
        ASSERT_EQ(row, 4);
    }
    {
        const auto args = spyMoved.takeFirst();
        const auto start = args.at(1).toInt();
        const auto end = args.at(2).toInt();
        const auto row = args.at(4).toInt();
        ASSERT_EQ(start, 0);
        ASSERT_EQ(end, 0);
        ASSERT_EQ(row, 4);
    }

    ASSERT_EQ(list.get(0), foo3);
    ASSERT_EQ(list.get(1), foo1);
    ASSERT_EQ(list.get(2), foo4);
    ASSERT_EQ(list.get(3), foo2);

    //'From' out of bound

    list.move(10, 2);
    ASSERT_EQ(spyAboutToMoved.count(), 0);
    ASSERT_EQ(spyMoved.count(), 0);
}

TEST_F(QOlmTestMockListFilled, RemoveQObject)
{
    QSignalSpy spyAboutToRemoved(&list, &qolm::QOlmBase::rowsAboutToBeRemoved);
    QSignalSpy spyRemoved(&list, &qolm::QOlmBase::rowsRemoved);

    EXPECT_CALL(list, onObjectAboutToBeRemoved(foo2, 1));
    EXPECT_CALL(list, onObjectRemoved(foo2, 1));

    ASSERT_EQ(list.size(), 4);

    bool expectedCallback = false;
    int expectedIndex = -1;
    QObject* expectedObject = nullptr;

    list.onRemoved(
        [&](QObject* object, int index)
        {
            ASSERT_TRUE(expectedCallback);
            ASSERT_EQ(expectedIndex, index);
            ASSERT_EQ(expectedObject, object);

            expectedCallback = false;
            expectedIndex = -1;
            expectedObject = nullptr;
        });

    expectedCallback = true;
    expectedIndex = 1;
    expectedObject = foo2;

    list.remove(foo2);

    EXPECT_FALSE(expectedCallback);
    ASSERT_EQ(spyAboutToRemoved.count(), 1);
    ASSERT_EQ(spyRemoved.count(), 1);
    {
        const auto args = spyAboutToRemoved.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 1);
        ASSERT_EQ(last, 1);
    }
    {
        const auto args = spyRemoved.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 1);
        ASSERT_EQ(last, 1);
    }

    ASSERT_EQ(list.get(0), foo1);
    ASSERT_EQ(list.get(1), foo3);
    ASSERT_EQ(list.get(2), foo4);

    //QObject not in the list

    list.remove(foo5);
    ASSERT_EQ(spyAboutToRemoved.count(), 0);
    ASSERT_EQ(spyRemoved.count(), 0);

    //Nullptr object to remove

    list.remove(nullptr);
    ASSERT_EQ(spyAboutToRemoved.count(), 0);
    ASSERT_EQ(spyRemoved.count(), 0);
}

TEST_F(QOlmTestMockListFilled, RemoveList)
{
    QSignalSpy spyAboutToRemoved(&list, &qolm::QOlmBase::rowsAboutToBeRemoved);
    QSignalSpy spyRemoved(&list, &qolm::QOlmBase::rowsRemoved);

    EXPECT_CALL(list, onObjectAboutToBeRemoved(foo2, 1));
    EXPECT_CALL(list, onObjectRemoved(foo2, 1));
    EXPECT_CALL(list, onObjectAboutToBeRemoved(foo3, 2));
    EXPECT_CALL(list, onObjectRemoved(foo3, 2));

    ASSERT_EQ(list.size(), 4);

    std::queue<std::pair<int, QObject*>> expectedCallback;

    list.onRemoved(
        [&](QObject* object, int index)
        {
            ASSERT_TRUE(!expectedCallback.empty());
            const auto [expectedIndex, expectedObject] = expectedCallback.front();
            ASSERT_EQ(expectedIndex, index);
            ASSERT_EQ(expectedObject, object);

            expectedCallback.pop();
        });

    expectedCallback.push(std::make_pair(2, foo3));
    expectedCallback.push(std::make_pair(1, foo2));

    list.remove({foo3, foo2});
    ASSERT_TRUE(expectedCallback.empty());
    ASSERT_EQ(spyAboutToRemoved.count(), 2);
    ASSERT_EQ(spyRemoved.count(), 2);
    {
        const auto args = spyAboutToRemoved.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 2);
        ASSERT_EQ(last, 2);
    }
    {
        const auto args = spyAboutToRemoved.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 1);
        ASSERT_EQ(last, 1);
    }
    {
        const auto args = spyRemoved.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 2);
        ASSERT_EQ(last, 2);
    }
    {
        const auto args = spyRemoved.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 1);
        ASSERT_EQ(last, 1);
    }
    ASSERT_EQ(list.size(), 2);
    ASSERT_EQ(list.get(0), foo1);
    ASSERT_EQ(list.get(1), foo4);

    //Remove an empty list

    list.remove(QList<QObject*>());
    ASSERT_EQ(spyAboutToRemoved.count(), 0);
    ASSERT_EQ(spyRemoved.count(), 0);

    //Remove a nullptr list

    list.remove(nullptr);
    ASSERT_EQ(spyAboutToRemoved.count(), 0);
    ASSERT_EQ(spyRemoved.count(), 0);
}
TEST_F(QOlmTestMockListFilled, RemoveIndex)
{
    QSignalSpy spyAboutToRemoved(&list, &qolm::QOlmBase::rowsAboutToBeRemoved);
    QSignalSpy spyRemoved(&list, &qolm::QOlmBase::rowsRemoved);

    EXPECT_CALL(list, onObjectAboutToBeRemoved(foo4, 3));
    EXPECT_CALL(list, onObjectRemoved(foo4, 3));
    EXPECT_CALL(list, onObjectAboutToBeRemoved(foo2, 1));
    EXPECT_CALL(list, onObjectRemoved(foo2, 1));
    EXPECT_CALL(list, onObjectAboutToBeRemoved(foo3, 2));
    EXPECT_CALL(list, onObjectRemoved(foo3, 2));

    ASSERT_EQ(list.size(), 4);

    std::queue<std::pair<int, QObject*>> expectedCallbackList;
    bool expectedCallback = false;
    int expectedIndex = -1;
    QObject* expectedObject = nullptr;

    list.onRemoved(
        [&](QObject* object, int index)
        {
            if(expectedCallback)
            {
                ASSERT_EQ(expectedIndex, index);
                ASSERT_EQ(expectedObject, object);

                expectedCallback = false;
                expectedIndex = -1;
                expectedObject = nullptr;
            }
            if(!expectedCallbackList.empty())
            {
                const auto [objectIndex, objectObject] = expectedCallbackList.front();
                ASSERT_EQ(objectIndex, index);
                ASSERT_EQ(objectObject, object);

                expectedCallbackList.pop();
            }
        });

    expectedCallback = true;
    expectedIndex = 3;
    expectedObject = foo4;

    list.remove(3);
    ASSERT_FALSE(expectedCallback);
    ASSERT_EQ(spyAboutToRemoved.count(), 1);
    ASSERT_EQ(spyRemoved.count(), 1);
    {
        const auto args = spyAboutToRemoved.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 3);
        ASSERT_EQ(last, 3);
    }
    {
        const auto args = spyRemoved.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 3);
        ASSERT_EQ(last, 3);
    }

    ASSERT_EQ(list.get(0), foo1);
    ASSERT_EQ(list.get(1), foo2);
    ASSERT_EQ(list.get(2), foo3);

    //Remove(int index, int count)
    expectedCallbackList.push(std::make_pair(1, foo2));
    expectedCallbackList.push(std::make_pair(2, foo3));
    //
    list.remove(1, 2);
    ASSERT_EQ(list.size(), 1);
    ASSERT_EQ(spyAboutToRemoved.count(), 1);
    ASSERT_EQ(spyRemoved.count(), 1);
    {
        const auto args = spyAboutToRemoved.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 1);
        ASSERT_EQ(last, 2);
    }
    {
        const auto args = spyRemoved.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 1);
        ASSERT_EQ(last, 2);
    }
    //
    //Index out of bound
    //
    list.remove(5);

    ASSERT_FALSE(expectedCallback || !expectedCallbackList.empty());
    ASSERT_EQ(spyAboutToRemoved.count(), 0);
    ASSERT_EQ(spyRemoved.count(), 0);
    //
    ASSERT_EQ(list.get(0), foo1);
}

TEST_F(QOlmTestMockListFilled, Clear)
{
    QSignalSpy spyAboutToRemoved(&list, &qolm::QOlmBase::rowsAboutToBeRemoved);
    QSignalSpy spyRemoved(&list, &qolm::QOlmBase::rowsRemoved);

    EXPECT_CALL(list, onObjectAboutToBeRemoved(foo1, 0));
    EXPECT_CALL(list, onObjectRemoved(foo1, 0));
    EXPECT_CALL(list, onObjectAboutToBeRemoved(foo2, 1));
    EXPECT_CALL(list, onObjectRemoved(foo2, 1));
    EXPECT_CALL(list, onObjectAboutToBeRemoved(foo3, 2));
    EXPECT_CALL(list, onObjectRemoved(foo3, 2));
    EXPECT_CALL(list, onObjectAboutToBeRemoved(foo4, 3));
    EXPECT_CALL(list, onObjectRemoved(foo4, 3));

    ASSERT_EQ(list.size(), 4);

    std::queue<std::pair<int, QObject*>> expectedCallback;

    list.onRemoved(
        [&](QObject* object, int index)
        {
            ASSERT_TRUE(!expectedCallback.empty());
            const auto [expectedIndex, expectedObject] = expectedCallback.front();
            ASSERT_EQ(expectedIndex, index);
            ASSERT_EQ(expectedObject, object);

            expectedCallback.pop();
        });

    expectedCallback.push(std::make_pair(0, foo1));
    expectedCallback.push(std::make_pair(1, foo2));
    expectedCallback.push(std::make_pair(2, foo3));
    expectedCallback.push(std::make_pair(3, foo4));

    list.clear();
    ASSERT_EQ(spyAboutToRemoved.count(), 1);
    ASSERT_EQ(spyRemoved.count(), 1);
    {
        const auto args = spyAboutToRemoved.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 0);
        ASSERT_EQ(last, 3);
    }
    {
        const auto args = spyRemoved.takeFirst();
        const auto first = args.at(1).toInt();
        const auto last = args.at(2).toInt();
        ASSERT_EQ(first, 0);
        ASSERT_EQ(last, 3);
    }

    ASSERT_EQ(list.size(), 0);
    ASSERT_TRUE(expectedCallback.empty());
}

TEST_F(QOlmTestMockListFilled, First) { ASSERT_EQ(list.first(), foo1); }

TEST_F(QOlmTestMock, First) { ASSERT_EQ(list.first(), nullptr); }

TEST_F(QOlmTestMockListFilled, Last) { ASSERT_EQ(list.last(), foo4); }

TEST_F(QOlmTestMock, Last) { ASSERT_EQ(list.last(), nullptr); }

TEST_F(QOlmTestMockListFilled, ToList) { ASSERT_EQ(list.toList(), QList<QObject*>({foo1, foo2, foo3, foo4})); }
