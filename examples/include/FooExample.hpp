#ifndef __FOO_EXAMPLE_HPP__
#define __FOO_EXAMPLE_HPP__

#include <QOlm/QOlm.hpp>

#include <QObject>
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
            Q_EMIT fooChanged();
        }
    }

    QString toString() const
    {
        return QStringLiteral("Foo: {foo=") + QString::number(getFoo()) + QStringLiteral("}");
    }
Q_SIGNALS:
    void fooChanged();
};

class FooList : public QOlm::QOlm<Foo>
{
    Q_OBJECT
public:
    FooList(QObject* parent = nullptr,
        const QList<QByteArray>& exposedRoles = {},
        const QByteArray& displayRole = {}) :
    QOlm<Foo>(parent, exposedRoles, displayRole)
    {
        // CALLBACK LISTEN DEMO
        onInserted([](const InsertedCallbackArgs& foo)
        {
	        // foo->foo can be directly accessed
            // foo.object gives a _Object*
            // foo.index gives inserted object index
            qInfo("callback : onInserted 0x%s : %d", qPrintable(QString::number(reinterpret_cast<quintptr>(static_cast<Foo*>(foo)), 16)), foo.index);
        });
        onRemoved([](const RemovedCallbackArgs& foo)
        {
	        // foo->foo can be directly accessed
            // foo.object gives a _Object*
            // foo.index gives removed object index
            qInfo("callback : onRemoved 0x%s : %d", qPrintable(QString::number(reinterpret_cast<quintptr>(static_cast<Foo*>(foo)), 16)), foo.index);
        });
        onMoved([](const MovedCallbackArgs& foo)
        {
	        // foo->foo can be directly accessed
            // foo.object gives a _Object*
            // foo.from gives previous object index
            // foo.to gives new object index
            qInfo("callback : onMoved 0x%s : %d %d", qPrintable(QString::number(reinterpret_cast<quintptr>(static_cast<Foo*>(foo)), 16)), foo.from, foo.to);
        });

        // SIGNAL LISTEN DEMO
        connect(this, &FooList::objectInserted, this,
            [](QObject* obj, int index)
            {
                qInfo("signal   : onInserted 0x%s : %d", qPrintable(QString::number(reinterpret_cast<quintptr>(qobject_cast<Foo*>(obj)), 16)), index);
            }
        );
        connect(this, &FooList::objectRemoved, this,
            [](QObject* obj, int index)
            {
                qInfo("signal   : onRemoved 0x%s : %d", qPrintable(QString::number(reinterpret_cast<quintptr>(qobject_cast<Foo*>(obj)), 16)), index);
            }
        );
        connect(this, &FooList::objectMoved, this,
            [](QObject* obj, int from, int to)
            {
                qInfo("signal   : onRemoved 0x%s : %d %d", qPrintable(QString::number(reinterpret_cast<quintptr>(qobject_cast<Foo*>(obj)), 16)), from, to);
            }
        );
    }

    QString toString() const
    {
        QString res = QStringLiteral("FooList : {");

        for (const auto it : *this)
            res += QStringLiteral("foo") + QString::number(it->getFoo()) + QStringLiteral(", ");
        res += QStringLiteral("}");

        return res;
    }

    // OVERRIDE LISTEN EXAMPLE
protected:
    void onObjectAboutToBeInserted(Foo* item, int row) override final
    {
        qInfo("override : onItemAboutToBeInserted 0x%s : %d", qPrintable(QString::number(reinterpret_cast<quintptr>(item), 16)), row);
    }
    void onObjectInserted(Foo* item, int row) override final
    {
        qInfo("override : onItemInserted 0x%s : %d", qPrintable(QString::number(reinterpret_cast<quintptr>(item), 16)), row);
    }
    void onObjectAboutToBeMoved(Foo* item, int src, int dest) override final
    {
        qInfo("override : onItemAboutToBeMoved 0x%s : %d %d", qPrintable(QString::number(reinterpret_cast<quintptr>(item), 16)), src, dest);
    }
    void onObjectMoved(Foo* item, int src, int dest) override final
    {
        qInfo("override : onItemMoved 0x%s : %d %d", qPrintable(QString::number(reinterpret_cast<quintptr>(item), 16)), src, dest);
    }
    void onObjectAboutToBeRemoved(Foo* item, int row) override final
    {
        qInfo("override : onItemAboutToBeRemoved 0x%s : %d", qPrintable(QString::number(reinterpret_cast<quintptr>(item), 16)), row);
    }
    void onObjectRemoved(Foo* item, int row) override final
    {
        qInfo("override : onItemRemoved 0x%s : %d", qPrintable(QString::number(reinterpret_cast<quintptr>(item), 16)), row);
    }
};

#endif
