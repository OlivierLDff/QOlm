#include <FooExample.hpp>

void appendExample()
{
    qInfo().noquote() << "Append example.";
    qInfo().noquote() << "We append foo1 at idx 0 and 2.";
    qInfo().noquote() << "We append foo2 at idx 1.";
    qInfo().noquote() << "foo1 value is 8.";
    qInfo().noquote() << "foo2 value is 9765.";
    FooList list;

    auto foo1 = new Foo();
    auto foo2 = new Foo();

    foo1->setFoo(8);
    foo2->setFoo(9765);

    list.append(foo1);
    list.append(foo2);
    list.append(foo1);

    qInfo().noquote() << list.toString();

    list.clear();

    delete foo1;
    delete foo2;
}

int main(int argc, char **argv)
{
    appendExample();
    return 0;
}