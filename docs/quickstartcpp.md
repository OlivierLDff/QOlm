---
layout: default
title: 🚀 Quick Start With C++
nav_order: 1
---

# 🚀 Quick Start With C++

Most of the time you want to store more than just `QObject` type, so create your custom type.

```cpp
class Foo : public QObject
{
    Q_OBJECT
public:
    Foo(QObject* parent = nullptr) : QObject(parent) {}

    int foo = 0;
};
```

Then use a `QOlm` based object with class inheritance or as a typedef.

```cpp
#include <Foo.hpp>
#include <QOlm/QOlm.hpp>

// Declare type
using FooList = qolm::QOlm<Foo>;

// Or create custom class
class FooList : public qolm::QOlm<Foo>
{
    Q_OBJECT
public:
    Foo(QObject* parent = nullptr,
        const QList<QByteArray> & exposedRoles = {},
        const QByteArray & displayRole = {}):
    qolm::QOlm<Foo>::(parent, exposedRoles, displayRole)
    {
    }
};
```

Then simply use it as a regular list.

## 📄 Api

### Insert elements

The object provide multiple way to insert an object in the list:

* `append` : Add an object at the end of the list.
* `prepend` : Add an object at the beginning of the list.
* `insert` : Insert object at requested offset.

Those three functions can also take a `QList<_Object*>` as a parameter entry to insert multiple object at once.

```cpp
FooList list;
Foo foo1;
Foo foo2;
Foo foo3;
Foo foo4;

// {foo1}
list.append(&foo1);

// {foo2, foo1}
list.prepend(&foo2);

// {foo2, foo1, foo3}
list.append(&foo3);

// {foo2, foo4, foo1, foo3}
list.insert(1, &foo4);
```

###  Remove elements

To remove an item, simply call the `remove` function, either with a pointer to the `_Object*` , or with the index of the object at which you want to remove.

All elements can also be removed using `clear` function.

```cpp
FooList list;
Foo foo1;
Foo foo2;
Foo foo3;
Foo foo4;
list.append({&foo1, &foo2, &foo3, &foo4});

// { &foo1, &foo2, &foo4 }
list.remove(&foo3);
// { &foo1, &foo2 }
list.remove(2);
// Remove all elements.
list.clear();
```

### Move elements

Elements can be moved within the list, without changing the list size.

* `move` from object at index `from` to index `to`.
* `moveUp`: Move object at index `index` to `index-1`. This function make sense when seeing the list in a `ListView` for example. It move the item to previous index.
* `moveDown`: Move object from `index` to `index+1`. This function make sense in a `ListView` in a column. It move the item to next index.
* `moveNext`: alias of `moveDown`.
* `movePrevious`: alias of `moveUp`.

```cpp
FooList list;
Foo foo1;
Foo foo2;
Foo foo3;
Foo foo4;
list.append({&foo1, &foo2, &foo3, &foo4});

// { &foo1, &foo3, &foo4, &foo2 }
list.move(1, 3);

// { &foo2, &foo1, &foo3, &foo4 }
list.move(3, 0);

// { &foo2, &foo3, &foo1, &foo4 }
list.movePrevious(2);

// { &foo2, &foo3, &foo4, &foo1 }
list.moveNext(2);
```

### Access element and get index

Multiple accessors can be used to get data.

* `get` : Get pointer to the `_Object*` at `index`.
* `indexOf` : Get the `index` from a `_Object*`
* `contains` : Get if a `_Object*` is present.
* `size`: Give the number of objects in the model
* `empty` : True if model is empty.

## 🏠 Object ownership

The library follow qt ownership rules. So when inserting an object without parent, the list take ownership of that object. When the same object is removed it will be `deleteLater`.

```c++
FooList list;
// list take ownership on new Foo
list.append(new Foo());
// Since FooList have ownership on the foo at index 0, it call deleteLater on it. No need to worry about memory management.
list.remove(0);
```

## 🔎 Observe Insert/Remove/Move

### Observe as QAbstractItemModel

The `qolm::QOlm` derived object can be observe for insertion and deletion like any qt model.

* **[rowsAboutToBeInserted](https://doc.qt.io/qt-5/qabstractitemmodel.html#rowsAboutToBeInserted)**
* **[rowsAboutToBeMoved](https://doc.qt.io/qt-5/qabstractitemmodel.html#rowsAboutToBeMoved)**
* **[rowsAboutToBeRemoved](https://doc.qt.io/qt-5/qabstractitemmodel.html#rowsAboutToBeRemoved)**
* **[rowsInserted](https://doc.qt.io/qt-5/qabstractitemmodel.html#rowsInserted)**
* **[rowsMoved](https://doc.qt.io/qt-5/qabstractitemmodel.html#rowsMoved)**
* **[rowsRemoved](https://doc.qt.io/qt-5/qabstractitemmodel.html#rowsRemoved)**

But those signals are not very convenient to use as a end user. That's why `QOlm` provide other way to observe the list.

### Observe thru signals

`qolm::QOlmBase` provide basic signal to react to `QObject` insert/remove/move operation.

* `onObjectInserted(QObject* object, int index)`
* `onObjectRemoved(QObject* object, int index)`
* `onObjectMoved(QObject* object, int from, int to)`

They are call when the model can safely be iterated. You can simply retrieve a correct pointer by using `qobject_cast<_Object*>(object)`.

### Function override observe.

Sometime it can be useful to do some processing before the whole world gets notify about our object operation. This method is only available if you define a custom list type.

```cpp
#include <Foo.hpp>
#include <QOlm/QOlm.hpp>

class FooList : public qolm::QOlm<Foo>
{
    Q_OBJECT
public:
    FooList(QObject* parent = nullptr,
        const QList<QByteArray> & exposedRoles = {},
        const QByteArray & displayRole = {}):
    QOlm<Foo>(parent, exposedRoles, displayRole)
    {
    }

protected:
    void onObjectAboutToBeInserted(_Object* item, int row) override
    {
        // Object is not yet inserted, do some preinsert operation on it.
    }
    void onObjectInserted(_Object* item, int row) override
    {
        // Object just got inserted, but no callback/signal have been called yet.
    }
    void onObjectAboutToBeMoved(_Object* item, int src, int dest) override
    {
        // Object haven't move yet, and no callback/signal have been called yet
    }
    void onObjectMoved(_Object* item, int src, int dest) override
    {
        // Object have been moved. No Callback/Signal have been called yet.
    }
    void onObjectAboutToBeRemoved(_Object* item, int row) override
    {
        // Object isn't removed yet, and no callback/signal have been called yet
    }
    void onObjectRemoved(_Object* item, int row) override
    {
        // Object have been removed. Callback/Signal have been called
    }
};
```

### Observe with callback

QOlm provide lambda connection with already `qobject_cast` objects. This is the preferred and easier way to observe the list

```cpp
FooList list;

// Preferred API, safer to use when giving a context
list.onInserted(&list, [](Foo* foo, int index){});
list.onInserted(&list, [](Foo* foo){});
list.onRemoved(&list, [](Foo* foo, int index){});
list.onRemoved(&list, [](Foo* foo){});
list.onMoved(&list, [](Foo* foo, int from, int to){});

// Should only be used when your callback doesn't require any context
list.onInserted([](Foo* foo, int index){});
list.onInserted([](Foo* foo){});
list.onRemoved([](Foo* foo, int index){});
list.onRemoved([](Foo* foo){});
list.onMoved([](Foo* foo, int from, int to){});
```

> When connecting without any `receiver`, this list is used as the context.

## Iterator

`QOlm` is compatible with modern iterator, you can simply do:

```cpp
FooList list;
for(const auto* foo : list)
{
    //foo->getFoo()
}
for(auto* foo : list)
{
    //foo->setFoo(12)
}
```
