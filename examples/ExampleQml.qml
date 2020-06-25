import QtQuick 2.12
import QtQuick.Window 2.12

import Example 1.0

Window
{
  id: window
  visible: true
  width: 640
  height: 480

  Foo
  {
    id: _foo1
    foo: 1
  }

  Foo
  {
    id: _foo2
    foo: 2
  }
  Component
  {
    id: _fooComponent
    Foo { }
  }

  Component
  {
    id: _nonFooComponent
    QtObject {}
  }

  Foo
  {
    id: _foo8
    foo: 8
  }
  Foo
  {
    id: _foo9
    foo: 9
  }
  Foo
  {
    id: _foo10
    foo: 10
  }

  function createFoos(model)
  {
      model.append(_foo1)
      model.append(_foo2)
      model.append(_fooComponent.createObject(this, {foo:5}))
      model.insert(3, _fooComponent.createObject(this, {foo:6}))
      model.prepend(_fooComponent.createObject(this, {foo:7}))

      model.append(_nonFooComponent.createObject(this, {foo:5}))
      model.insert(3, _nonFooComponent.createObject(this, {foo:6}))
      model.prepend(_nonFooComponent.createObject(this, {foo:7}))

      model.append([_foo8, _foo9, _foo10])
      model.prepend([_fooComponent.createObject(this, {foo:11}), _fooComponent.createObject(this, {foo:12}), _fooComponent.createObject(this, {foo:13})])
      model.insert(7, [_fooComponent.createObject(this, {foo:14}), _fooComponent.createObject(this, {foo:15}), _fooComponent.createObject(this, {foo:16})])
  }

  Row
  {
    anchors.fill: parent
    ListView
    {
      width: 200
      height: parent.height
      model: FooList
      {
        Foo
        {
          foo: 3
        }
        Foo
        {
          foo: 4
        }
      }

      delegate: Text
      {
        readonly property Foo currentFoo: qtObject
        text: `foo: ${foo} (${currentFoo.toString()})`
      }

      Component.onCompleted: () => window.createFoos(model)
    }
    ListView
    {
      width: 200
      height: parent.height
      model: FooListTemplate
      {
        Foo
        {
          foo: 3
        }
        Foo
        {
          foo: 4
        }
      }

      delegate: Text
      {
        readonly property Foo currentFoo: qtObject
        text: `foo: ${foo} (${currentFoo.toString()})`
      }

      Component.onCompleted: () => window.createFoos(model)
    }
    ListView
    {
      width: 200
      height: parent.height
      model: QtObjectList
      {
        QtObject
        {
          property int foo: 3
        }
        Foo
        {
          foo: 4
        }
      }

      delegate: Text
      {
        text: `foo: ${qtObject.foo} (${qtObject.toString()})`
      }

      Component.onCompleted: () => window.createFoos(model)
    }
  }
}
