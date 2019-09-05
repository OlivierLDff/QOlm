#ifndef __FOO_EXAMPLE_HPP__
#define __FOO_EXAMPLE_HPP__

#include <QObject>
#include <ObjectListModel.hpp>
#include <QDebug>

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

    QString toString() const
    {
        return QStringLiteral("Foo: {foo=") + QString::number(getFoo()) + QStringLiteral("}");
    }
signals:
    void fooChanged();
};

class FooList : public Olm::ObjectList<Foo>
{
    Q_OBJECT

public:
    QString toString() const
    {
        QString res = QStringLiteral("FooList : {") + "\n";
        int idx = 0;

        // Iterate thru the list and retrieve pointer to QObject.
        for(const auto it : *this) if(it)
        {
            res += QStringLiteral("[") + QString::number(idx) + QStringLiteral("] : ") +
                it->toString() + "\n";
            ++idx;
        }
        res += QStringLiteral("}");
        return res;
    }
};

#endif