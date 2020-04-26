#ifndef __FOO_LIST_HPP__
#define __FOO_LIST_HPP__

#include <QOlm/QOlm.hpp>
#include <Foo.hpp>

class FooList : public qolm::QOlm<Foo>
{
    Q_OBJECT
};

#endif
