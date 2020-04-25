#include <FooExample.hpp>

void insertExample()
{
    qInfo("\n -- Insert Example\n");
    FooList list;
    Foo foo1;
    foo1.setFoo(1);
    Foo foo2;
    foo2.setFoo(2);
    Foo foo3;
    foo3.setFoo(3);
    Foo foo4;
    foo4.setFoo(4);

    qInfo("list     : %s", qPrintable(list.toString()));

    // {foo1}
    qInfo("function : append foo1");
    list.append(&foo1);
    qInfo("list     : %s", qPrintable(list.toString()));

    // {foo2, foo1}
    qInfo("function : prepend foo2");
    list.prepend(&foo2);
    qInfo("list     : %s", qPrintable(list.toString()));

    // {foo2, foo1, foo3}
    qInfo("function : append foo3");
    list.append(&foo3);
    qInfo("list     : %s", qPrintable(list.toString()));

    // {foo2, foo4, foo1, foo3}
    qInfo("function : insert foo4 at 1");
    list.insert(1, &foo4);
    qInfo("list     : %s", qPrintable(list.toString()));
}

void removeExample()
{
    qInfo("\n -- Remove Example\n");
    FooList list;
    Foo foo1;
    foo1.setFoo(1);
    Foo foo2;
    foo2.setFoo(2);
    Foo foo3;
    foo3.setFoo(3);
    Foo foo4;
    foo4.setFoo(4);
    list.append({ &foo1, &foo2, &foo3, &foo4 });

    qInfo("list     : %s", qPrintable(list.toString()));

    // { &foo1, &foo2, &foo4 }
    qInfo("function : Remove foo3");
    list.remove(&foo3);
    qInfo("list     : %s", qPrintable(list.toString()));
    // { &foo1, &foo2 }
    qInfo("function : Remove at 2");
    list.remove(2);
    qInfo("list     : %s", qPrintable(list.toString()));

    // { }
    qInfo("function : Remove at foo1 & foo2");
    list.remove({&foo1, &foo2});
    qInfo("list     : %s", qPrintable(list.toString()));
}

void moveExample()
{
    qInfo("\n -- Move Example\n");
    FooList list;
    Foo foo1;
    foo1.setFoo(1);
    Foo foo2;
    foo2.setFoo(2);
    Foo foo3;
    foo3.setFoo(3);
    Foo foo4;
    foo4.setFoo(4);
    list.append({ &foo1, &foo2, &foo3, &foo4 });

    qInfo("list     : %s", qPrintable(list.toString()));

    // { &foo1, &foo3, &foo4, &foo2 }
    qInfo("function : Move from 1 to 3");
    list.move(1, 3);
    qInfo("list     : %s", qPrintable(list.toString()));

    // { &foo2, &foo1, &foo3, &foo4 }
    qInfo("function : Move from 3 to 0");
    list.move(3, 0);
    qInfo("list     : %s", qPrintable(list.toString()));

    // { &foo2, &foo3, &foo1, &foo4 }
    qInfo("function : Move from 2 to 1");
    list.moveUp(2);
    qInfo("list     : %s", qPrintable(list.toString()));

    // { &foo2, &foo3, &foo4, &foo1 }
    qInfo("function : Move from 2 to 3");
    list.moveDown(2);
    qInfo("list     : %s", qPrintable(list.toString()));
}

int main(int argc, char **argv)
{
    insertExample();
    removeExample();
    moveExample();
    return 0;
}
