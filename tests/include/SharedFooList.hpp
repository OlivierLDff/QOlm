#ifndef __SHARED_FOO_LIST_HPP__
#define __SHARED_FOO_LIST_HPP__

#include <ObjectListModel.hpp>
#include <Foo.hpp>

class SharedFooList : public Olm::SharedObjectList<Foo>
{
    Q_OBJECT
};

#endif