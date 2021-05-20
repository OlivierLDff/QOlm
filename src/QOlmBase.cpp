// How can I use this ?
//
// Well, license here is pretty "super-open", not even copy-left or copy-right, basically its close to the WTFPL, just use it as you want, as it's the most practical to you :
//
// If you want to use it as GIT submodule and compile it in your app, do it.
// If you prefer separate project as a shared library, do it.
// If you need to modify the project to be able to integrate in you app (opensource or not), do it.
// If you want to share you work on the library, thanks a lot, but if you don't, no problem.
// If you think about some weird case I didn't talk about, well, do whatever you want, I don't need to know about it.
// If you like it so much you want to spread the word, thank you, you're welcome.
// Enjoy !

#include <QOlm/QOlmBase.hpp>

using namespace qolm;

QOlmBase::QOlmBase(QObject* parent) : QAbstractListModel(parent) {}

int QOlmBase::size() const { return count(); }

int QOlmBase::length() const { return count(); }

bool QOlmBase::empty() const { return count() == 0; }

void QOlmBase::moveNext(const int index) { moveDown(index); }

void QOlmBase::movePrevious(const int index) { moveUp(index); }

QQmlListProperty<QObject> QOlmBase::defaultChildren()
{
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    return QQmlListProperty<QObject>(this, this, &QOlmBase::appendDefaultChild, &QOlmBase::defaultChildrenCount,
        &QOlmBase::defaultChild, &QOlmBase::clearDefaultChildren);
#else
    return QQmlListProperty<QObject>(this, this, &QOlmBase::appendDefaultChild, &QOlmBase::defaultChildrenCount,
        &QOlmBase::defaultChild, &QOlmBase::clearDefaultChildren, &QOlmBase::replaceDefaultChild,
        &QOlmBase::removeLastChild);
#endif
}

void QOlmBase::appendDefaultChild(QQmlListProperty<QObject>* list, QObject* child)
{
    return reinterpret_cast<QOlmBase*>(list->data)->appendDefaultChild(child);
}

QOlmBase::qolm_size_type QOlmBase::defaultChildrenCount(QQmlListProperty<QObject>* list)
{
    return reinterpret_cast<QOlmBase*>(list->data)->defaultChildrenCount();
}

QObject* QOlmBase::defaultChild(QQmlListProperty<QObject>* list, qolm_size_type index)
{
    return reinterpret_cast<QOlmBase*>(list->data)->defaultChild(index);
}

void QOlmBase::clearDefaultChildren(QQmlListProperty<QObject>* list)
{
    return reinterpret_cast<QOlmBase*>(list->data)->clearDefaultChildren();
}

void QOlmBase::replaceDefaultChild(QQmlListProperty<QObject>* list, qolm_size_type index, QObject* child)
{
    return reinterpret_cast<QOlmBase*>(list->data)->replaceDefaultChild(index, child);
}

void QOlmBase::removeLastChild(QQmlListProperty<QObject>* list)
{
    return reinterpret_cast<QOlmBase*>(list->data)->removeLastChild();
}
