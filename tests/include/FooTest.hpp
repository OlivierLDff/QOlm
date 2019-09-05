#ifndef __FOO_TEST_HPP__
#define __FOO_TEST_HPP__

#include <QObject>
#include <ObjectListModel.hpp>

class Foo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int foo READ getFoo WRITE setFoo NOTIFY fooChanged)

    int _foo = 0;

public:
    Foo(QObject* parent = nullptr) : QObject(parent) {}
    int getFoo() const { return _foo; }
    void setFoo(int foo)
    {
        if(foo != _foo)
        {
            _foo = foo;
            emit fooChanged();
        }
    }
signals:
    void fooChanged();
};

class FooList : public Olm::ObjectList<Foo>
{
    Q_OBJECT
};

#endif