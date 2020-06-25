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
    return {this, this, &QOlmBase::appendDefaultChild, &QOlmBase::defaultChildrenCount, &QOlmBase::defaultChild,
        &QOlmBase::clearDefaultChildren, &QOlmBase::replaceDefaultChild, &QOlmBase::removeLastChild};
}

void QOlmBase::appendDefaultChild(QQmlListProperty<QObject>* list, QObject* child)
{
    return reinterpret_cast<QOlmBase*>(list->data)->appendDefaultChild(child);
}

int QOlmBase::defaultChildrenCount(QQmlListProperty<QObject>* list)
{
    return reinterpret_cast<QOlmBase*>(list->data)->defaultChildrenCount();
}

QObject* QOlmBase::defaultChild(QQmlListProperty<QObject>* list, int index)
{
    return reinterpret_cast<QOlmBase*>(list->data)->defaultChild(index);
}

void QOlmBase::clearDefaultChildren(QQmlListProperty<QObject>* list)
{
    return reinterpret_cast<QOlmBase*>(list->data)->clearDefaultChildren();
}

void QOlmBase::replaceDefaultChild(QQmlListProperty<QObject>* list, int index, QObject* child)
{
    return reinterpret_cast<QOlmBase*>(list->data)->replaceDefaultChild(index, child);
}

void QOlmBase::removeLastChild(QQmlListProperty<QObject>* list)
{
    return reinterpret_cast<QOlmBase*>(list->data)->removeLastChild();
}
