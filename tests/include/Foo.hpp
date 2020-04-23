#ifndef __FOO_HPP__
#define __FOO_HPP__

#include <QObject>

class Foo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int foo READ getFoo WRITE setFoo NOTIFY fooChanged)

    int _foo = 0;

public:
    Foo(QObject* parent = nullptr) : QObject(parent)
    {
        //qDebug("Constructor");        
    }
    ~Foo()
    {
        //qDebug("Destructor");
    }
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

#endif