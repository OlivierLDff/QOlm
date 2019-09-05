#ifndef __FOO_LIST_HPP__
#define __FOO_LIST_HPP__

#include <ObjectListModel.hpp>
#include <Foo.hpp>

class FooList : public Olm::ObjectList<Foo>
{
    Q_OBJECT
};

#endif