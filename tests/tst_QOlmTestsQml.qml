import QtQuick 2.0
import QtTest 1.2

import QOlmTestsQml 1.0

TestCase
{
  name: "QOlmTestsQml"

  Component
  {
    id: qtListComponent
    QtObjectList {} // === qolm::QOlm<QObject>
  }

  Component
  {
    id: qtObjectComponent
    QtObject {} // === QObject
  }

  function init_data()
  {
    const listFilled = qtListComponent.createObject(null, {objectName: "listFilled"})
    const list = qtListComponent.createObject(null, {objectName: "list"})
    const foo1 = qtObjectComponent.createObject(null, {objectName: "foo1"})
    const foo2 = qtObjectComponent.createObject(null, {objectName: "foo2"})
    const foo3 = qtObjectComponent.createObject(null, {objectName: "foo3"})
    const foo4 = qtObjectComponent.createObject(null, {objectName: "foo4"})
    const foo5 = qtObjectComponent.createObject(null, {objectName: "foo5"})

    listFilled.append([foo1, foo2, foo3, foo4]);

    return [{tag: "datalist", listFilled: listFilled, list: list, foo1: foo1, foo2: foo2, foo3: foo3, foo4: foo4, foo5: foo5}]
  }

  function test_Append(data)
  {
    data.list.append(data.foo1);

    compare(data.list.count, 1);
    compare(data.list.get(0), data.foo1);

    data.list.append(null);
    compare(data.list.count,1)
    compare(data.list.get(1), null);

    data.list.append([data.foo1, data.foo2, data.foo3]);
    compare(data.list.count, 4);
    compare(data.list.get(0),data.foo1);
    compare(data.list.get(1),data.foo1);
    compare(data.list.get(2),data.foo2);
    compare(data.list.get(3),data.foo3);

    const myList2 = [];
    data.list.append(myList2);
    compare(data.list.get(4), null)
  }

  function test_Prepend(data)
  {
    data.list.prepend(data.foo1);
    compare(data.list.count, 1);
    compare(data.list.get(0), data.foo1);

    data.list.prepend(null);
    compare(data.list.count,1)
    compare(data.list.get(0), data.foo1);

    data.list.prepend([data.foo1, data.foo2, data.foo3]);
    compare(data.list.get(0),data.foo1);
    compare(data.list.get(1),data.foo2);
    compare(data.list.get(2),data.foo3);
    compare(data.list.get(3),data.foo1);

    const myList2 = [];
    data.list.prepend(myList2);
    compare(data.list.get(0), data.foo1)
  }

  function test_Insert(data)
  {
    data.list.insert(0, data.foo1);
    compare(data.list.count, 1);
    compare(data.list.get(0), data.foo1);

    data.list.insert(1, null);
    compare(data.list.count,1)
    compare(data.list.get(1), null);

    data.list.insert(0, [data.foo1, data.foo2, data.foo3]);
    compare(data.list.get(0),data.foo1);
    compare(data.list.get(1),data.foo2);
    compare(data.list.get(2),data.foo3);
    compare(data.list.get(3),data.foo1);

    const myList2 = [];
    data.list.insert(2, myList2);
    compare(data.list.get(0), data.foo1);
 }

 function test_Remove(data)
 {
    data.listFilled.remove(0);

    compare(data.listFilled.count, 3);
    compare(data.listFilled.get(0), data.foo2);
    compare(data.listFilled.get(1), data.foo3);
    compare(data.listFilled.get(2), data.foo4);

    data.listFilled.remove(data.foo2);

    compare(data.listFilled.count,2)
    compare(data.listFilled.get(0), data.foo3);
    compare(data.listFilled.get(1), data.foo4);

    data.listFilled.remove([data.foo3, data.foo4]);

    compare(data.listFilled.count,0)
 }

 function test_Contains(data)
 {
    verify(data.listFilled.contains(data.foo2));

    //Expected QWARN cases

    verify(data.listFilled.contains(data.foo5) == false);
    verify(data.listFilled.contains(data.foo6) == false);
 }

 function test_IndexOf(data)
 {
    compare(data.listFilled.indexOf(data.foo1),0);
    compare(data.listFilled.indexOf(data.foo2),1);
    compare(data.listFilled.indexOf(data.foo3),2);
    compare(data.listFilled.indexOf(data.foo4),3);

    //Expected QWARN cases

    compare(data.listFilled.indexOf(data.foo5),-1);
    compare(data.listFilled.indexOf(data.foo6),-1);

 }

 function test_MoveUp(data)
 {
    data.listFilled.moveUp(1);

    compare(data.listFilled.get(1), data.foo1);
    compare(data.listFilled.get(0), data.foo2);

    //Expected QWARN cases

    data.listFilled.moveUp(0);
    data.listFilled.moveUp(4);
 }

 function test_MoveDown(data)
 {
    data.listFilled.moveDown(2);

    compare(data.listFilled.get(2), data.foo4);
    compare(data.listFilled.get(3), data.foo3);

    //Expected QWARN cases

    data.listFilled.moveDown(-1);
    data.listFilled.moveDown(3);
 }

}

