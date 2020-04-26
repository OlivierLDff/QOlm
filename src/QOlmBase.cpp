#include <QOlm/QOlmBase.hpp>

using namespace qolm;

QOlmBase::QOlmBase(QObject* parent) : QAbstractListModel(parent) {}

int QOlmBase::size() const { return count(); }

int QOlmBase::length() const { return count(); }

bool QOlmBase::empty() const { return count() == 0; }

void QOlmBase::moveNext(const int index) { moveDown(index); }

void QOlmBase::movePrevious(const int index) { moveUp(index); }
