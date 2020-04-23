---
layout: default
title: Quick Start With Qml
nav_order: 2
---

# Getting Started Qml

The same api as the c++ work in qml. Every `Q_PROPERTY` are exposed as role, and another role `qtObject` allow to access the `QObject*`.

For the following example to work `Foo` and `FooList` need to be registered to the qml system.

```js
import QtQuick 2.0
import MyFoo 1.0

ListView {
    width: 180; height: 200
    FooList { id: _fooList }

    model: _fooList
    delegate: Text
    {
        // Access role qtObject and cast it to our type
        property Foo fooObj : model.qtObject
        text: index + ": " +
            fooObj.foo + // Access via casted object
            + ", " +
            foo // Access via role
    }

    Component.onCompleted:
    {
        _fooList.append(new Foo())
        _fooList.insert(1, new Foo())
        _fooList.prepend(new Foo())
    }
}
```

If you need to filter exposed roles, then use the constructor arguments. Same to set a display role.

```cpp
//QOlm(QObject* parent = nullptr,
//   const QList<QByteArray> & exposedRoles = {},
//   const QByteArray & displayRole = {})

// The following code expose foo as exposedRoles, and foo as Qt::DisplayRole
FooList list(nullptr, {"foo"}, "foo");
```

It is recommended to only expose role that are required for `QSortFilterProxyModel ` subclass. And use native signal to property for property that often change.