#ifndef __QOLM_QOLM_HPP__
#define __QOLM_QOLM_HPP__

#include <QOlm/QOlmBase.hpp>

#include <QByteArray>
#include <QDebug>
#include <QHash>
#include <QList>
#include <QMetaMethod>
#include <QMetaObject>
#include <QMetaProperty>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QVector>

#include <eventpp/callbacklist.h>

#include <algorithm>

namespace qolm {

template<class _Object>
class QOlm : public QOlmBase
{
    // ──────── CONSTRUCTOR ──────────
public:
    /**
     * @brief      Build a list object
     *
     * @param      parent        The qt parent. This can be left null, but it's recommended
     * to always use a qt parent. This allow automatic memory management.
     * When an object is destroyed, all the children are also destroyed.
     * @param[in]  exposedRoles  The exposed roles list. It's a list of string with named
     * properties that should be exposed as roles. By defaults all the Q_PROPERTY declared
     * attributes are exposed as roles. Some roles are blacklisted.
     * * id
     * * index
     * * class
     * * model
     * * modelData
     * These name should never be used as Q_PROPERTY in general because they conflicts QML basics keywords.
     * @param[in]  displayRole   The display role. This is the role used to display the object/content of the object by default when no roles is specified.
     */
    explicit QOlm(
        QObject* parent = nullptr, const QList<QByteArray>& exposedRoles = {}, const QByteArray& displayRole = {}) :
        QOlmBase(parent),
        _count(0), _displayRoleName(displayRole), _metaObj(_Object::staticMetaObject)
    {
        // Keep a track of black list rolename that are not compatible with Qml, they should never be used
        static QSet<QByteArray> roleNamesBlacklist;
        if(roleNamesBlacklist.isEmpty())
        {
            roleNamesBlacklist << QByteArrayLiteral("id") << QByteArrayLiteral("index") << QByteArrayLiteral("class")
                               << QByteArrayLiteral("model") << QByteArrayLiteral("modelData");
        }

        // Set handler that handle every property changed
        static const char* HANDLER = "onItemPropertyChanged()";
        _handler = QOlmBase::metaObject()->method(QOlmBase::metaObject()->indexOfMethod(HANDLER));

        // Force a display role the the role map
        if(!displayRole.isEmpty())
        {
            _roles.insert(Qt::DisplayRole, QByteArrayLiteral("display"));
        }
        // Return a pointer to the qtObject as the base Role. This point is essential
        _roles.insert(baseRole(), QByteArrayLiteral("qtObject"));

        // Number of attribute declare with the Q_PROPERTY flags
        const int len = _metaObj.propertyCount();
        // For every property in the ItemType
        for(int propertyIdx = 0, role = (baseRole() + 1); propertyIdx < len; propertyIdx++, role++)
        {
            QMetaProperty metaProp = _metaObj.property(propertyIdx);
            const QByteArray propName = QByteArray(metaProp.name());
            // Only expose the property as a role if:
            // - It isn't blacklisted(id, index, class, model, modelData)
            // - When exposedRoles is empty we expose every property
            // - When exposedRoles isn't empty we only expose the property asked by the user
            if(!roleNamesBlacklist.contains(propName) && (exposedRoles.empty() || exposedRoles.contains(propName)))
            {
                _roles.insert(role, propName);
                // If there is a notify signal associated with the Q_PROPERTY we keep a track of it for fast lookup
                if(metaProp.hasNotifySignal())
                {
                    _signalIdxToRole.insert(metaProp.notifySignalIndex(), role);
                }
            }
            else if(roleNamesBlacklist.contains(propName))
            {
                qWarning() << "Can't have" << propName << "as a role name in" << qPrintable(templateClassName())
                           << ", because it's a blacklisted keywork in QML!. "
                              "Please don't use any of the following words "
                              "when declaring your Q_PROPERTY:(id, index, "
                              "class, model, modelData)";
            }
        }
    }

public:
    QString templateClassName() const
    {
        static const QString CLASS_NAME = (QStringLiteral("QOlm<") + _metaObj.className() + QStringLiteral(">"));
        return CLASS_NAME;
    }

    // ──────── ABSTRACT MODEL OVERRIDE ──────────
public:
    bool setData(const QModelIndex& modelIndex, const QVariant& value, int role) override final
    {
        bool ret = false;
        _Object* item = at(modelIndex.row());
        const QByteArray roleName = (role != Qt::DisplayRole ? _roles.value(role, emptyBA()) : _displayRoleName);
        if(item != nullptr && role != baseRole() && !roleName.isEmpty())
            ret = item->setProperty(roleName, value);
        return ret;
    }

    QVariant data(const QModelIndex& modelIndex, int role) const override final
    {
        QVariant ret;
        _Object* item = at(modelIndex.row());
        const QByteArray roleName = (role != Qt::DisplayRole ? _roles.value(role, emptyBA()) : _displayRoleName);
        if(item != nullptr && !roleName.isEmpty())
            ret.setValue(
                role != baseRole() ? item->property(roleName) : QVariant::fromValue(static_cast<QObject*>(item)));
        return ret;
    }

    QHash<int, QByteArray> roleNames() const final { return _roles; }

    // ──────── ABSTRACT MODEL PRIVATE ──────────
protected:
    static const QString& emptyStr()
    {
        static const QString ret = QStringLiteral("");
        return ret;
    }
    static const QByteArray& emptyBA()
    {
        static const QByteArray ret = QByteArrayLiteral("");
        return ret;
    }
    static const QModelIndex& noParent()
    {
        static const QModelIndex ret = QModelIndex();
        return ret;
    }
    static const int& baseRole()
    {
        static const int role = Qt::UserRole;
        return role;
    }
    int rowCount(const QModelIndex& parent = QModelIndex()) const override final
    {
        return (!parent.isValid() ? _objects.count() : 0);
    }
    void referenceItem(_Object* item)
    {
        if(item != nullptr)
        {
            if(!item->parent())
                item->setParent(this);

            for(QHash<int, int>::const_iterator it = _signalIdxToRole.constBegin(); it != _signalIdxToRole.constEnd();
                ++it)
                connect(item, item->metaObject()->method(it.key()), this, _handler, Qt::UniqueConnection);
        }
    }
    void dereferenceItem(_Object* item)
    {
        if(item != nullptr)
        {
            disconnect(this, nullptr, item, nullptr);
            disconnect(item, nullptr, this, nullptr);

            if(item->parent() == this)
            {
                item->deleteLater();
            }
        }
    }
    void onItemPropertyChanged() override final
    {
        _Object* item = qobject_cast<_Object*>(sender());
        const int row = _objects.indexOf(item);
        const int sig = senderSignalIndex();
        const int role = _signalIdxToRole.value(sig, -1);
        if(row >= 0 && role >= 0)
        {
            const QModelIndex index = QAbstractListModel::index(row, 0, noParent());
            QVector<int> rolesList;
            rolesList.append(role);

            if(_roles.value(role) == _displayRoleName)
                rolesList.append(Qt::DisplayRole);

            Q_EMIT dataChanged(index, index, rolesList);
        }
    }
    void updateCounter()
    {
        if(_count != _objects.count())
        {
            _count = _objects.count();
            Q_EMIT countChanged(_count);
        }
    }

    // ──────── ITERATOR ──────────
public:
    using const_iterator = typename QList<_Object*>::const_iterator;
    const_iterator begin() const { return _objects.begin(); }
    const_iterator end() const { return _objects.end(); }
    const_iterator constBegin() const { return _objects.constBegin(); }
    const_iterator constEnd() const { return _objects.constEnd(); }

    // ──────── C++ API ──────────
public:
    _Object* at(int index) const { return get(index); }
    _Object* get(int index) const
    {
        if(index < 0 || index >= _objects.size())
        {
            qWarning() << "The index" << index << "is out of bound.";
            return nullptr;
        }
        return _objects.at(index);
    }

    QObject* get(QJSValue index) const override
    {
        const auto i = index.toInt();
        return get(i);
    }

    int roleForName(const QByteArray& name) const final { return _roles.key(name, -1); }
    int count() const final { return _count; }

    bool contains(_Object* object) const { return _objects.contains(object); }
    int indexOf(_Object* object) const
    {
        if(!object)
        {
            qWarning() << "Can't find the index of a nullptr QObject";
            return -1;
        }
        if(!_objects.contains(object))
        {
            qWarning() << "This QObject" << object << "isn't in the list.";
            return -1;
        }
        return _objects.indexOf(object);
    }

    void append(_Object* object)
    {
        if(object != nullptr)
        {
            const int pos = _objects.count();
            objectAboutToBeInsertedNotify(object, pos);
            beginInsertRows(noParent(), pos, pos);
            _objects.append(object);
            referenceItem(object);
            updateCounter();
            endInsertRows();
            objectInsertedNotify(object, pos);
        }
        else
        {
            qWarning() << "Can't append a null Object";
        }
    }

    void prepend(_Object* item)
    {
        if(item != nullptr)
        {
            objectAboutToBeInsertedNotify(item, 0);
            beginInsertRows(noParent(), 0, 0);
            _objects.prepend(item);
            referenceItem(item);
            updateCounter();
            endInsertRows();
            objectInsertedNotify(item, 0);
        }
        else
        {
            qWarning() << "Can't prepend a null object";
        }
    }
    void insert(int index, _Object* item)
    {
        if(index > count())
        {
            qWarning() << "index " << index << " is greater than count "<< count() <<". "
                << "The item will be inserted at the end of the list";
            index = count();
        }
        else if(index < 0)
        {
            qWarning() << "index " << index << " is lower than 0. "
                       << "The item will be inserted at the beginning of the list";
            index = 0;
        }

        if(item != nullptr)
        {
            objectAboutToBeInsertedNotify(item, index);
            beginInsertRows(noParent(), index, index);
            _objects.insert(index, item);
            referenceItem(item);
            updateCounter();
            endInsertRows();
            objectInsertedNotify(item, index);
        }
        else
        {
            qWarning() << "Can't insert a null Object";
        }
    }
    void append(const QList<_Object*>& itemList)
    {
        if(!itemList.isEmpty())
        {
            const int pos = _objects.count();
            for(int i = 0; i < itemList.count(); ++i) objectAboutToBeInsertedNotify(itemList.at(i), i + pos);

            beginInsertRows(noParent(), pos, pos + itemList.count() - 1);
            _objects.reserve(_objects.count() + itemList.count());
            _objects.append(itemList);
            for(const auto item: *this) { referenceItem(item); }
            updateCounter();
            endInsertRows();
            for(int i = 0; i < itemList.count(); ++i) objectInsertedNotify(itemList.at(i), i + pos);
        }
        else
        {
            qWarning() << "Can't append an empty list";
        }
    }
    void prepend(const QList<_Object*>& itemList)
    {
        if(!itemList.isEmpty())
        {
            for(int i = 0; i < itemList.count(); ++i) objectAboutToBeInsertedNotify(itemList.at(i), i);
            beginInsertRows(noParent(), 0, itemList.count() - 1);
            _objects.reserve(_objects.count() + itemList.count());
            int offset = 0;
            for(const auto item: itemList)
            {
                _objects.insert(offset, item);
                referenceItem(item);
                offset++;
            }
            updateCounter();
            endInsertRows();
            for(int i = 0; i < itemList.count(); ++i) objectInsertedNotify(itemList.at(i), i);
        }
        else
        {
            qWarning() << "Can't prepend an empty list";
        }
    }
    void insert(int idx, const QList<_Object*>& itemList)
    {
        if(!itemList.isEmpty())
        {
            for(int i = 0; i < itemList.count(); ++i) objectAboutToBeInsertedNotify(itemList.at(i), i + idx);
            beginInsertRows(noParent(), idx, idx + itemList.count() - 1);
            _objects.reserve(_objects.count() + itemList.count());
            int offset = 0;
            for(const auto item: itemList)
            {
                _objects.insert(idx + offset, item);
                referenceItem(item);
                offset++;
            }
            updateCounter();
            endInsertRows();
            for(int i = 0; i < itemList.count(); ++i) objectInsertedNotify(itemList.at(i), i + idx);
        }
        else
        {
            qWarning() << "Can't insert an empty list";
        }
    }
    void move(int from, int to) override final
    {
        if(from >= 0 && from < count())
        {
            const auto clampedTo = std::clamp(to, 0, count() - 1);
            if(clampedTo != to)
            {
                qWarning() << "'to'" << to << " in move operation have been clamped to" << clampedTo;
                to = clampedTo;
                if(from == to)
                {
                    qWarning() << "Can't move object from" << from << "to" << to << "because from == to";
                    return;
                }
            }

            const auto object = _objects.at(from);
            objectAboutToBeMovedNotify(object, from, to);
            beginMoveRows(noParent(), from, from, noParent(), (from < to ? to + 1 : to));
            _objects.move(from, to);
            endMoveRows();
            objectMovedNotify(object, from, to);
        }
        else
        {
            qWarning() << "'From'"<< from << "is out of bound";
        }
    }
    void remove(_Object* object)
    {
        if(object != nullptr)
            remove(indexOf(object));
        qWarning() << "Can't remove a null pointer Object of a list";
    }
    void remove(const QList<_Object*>& objects)
    {
        for(const auto object: objects) remove(object);
    }
    void remove(int index, int count = 1)
    {
        if(index >= 0 && (index + count - 1) < _objects.size())
        {
            QList<_Object*> tempList;
            for(const auto item: *this){ tempList.append(item); }
            for(int i = 0; i < count; ++i) objectAboutToBeRemovedNotify(tempList.at(index + i), index + i);
            beginRemoveRows(noParent(), index, index + count - 1);
            for(int i = 0; i < count; ++i)
            {
                _Object* item = _objects.takeAt(index);
                dereferenceItem(item);
            }
            updateCounter();
            endRemoveRows();
            for(int i = 0; i < count; ++i) objectRemovedNotify(tempList.at(index + i), index + i);
        }
        else
        {
            qWarning() << "Can't remove an object whose index is out of bound";
        }
    }
    void clear() override final
    {
        if(!_objects.isEmpty())
        {
            QList<_Object*> tempList;
            for(int i = 0; i < _objects.count(); ++i) objectAboutToBeRemovedNotify(_objects.at(i), i);
            beginRemoveRows(noParent(), 0, _objects.count() - 1);
            for(const auto item: *this)
            {
                dereferenceItem(item);
                tempList.append(item);
            }
            _objects.clear();
            updateCounter();
            endRemoveRows();
            for(int i = 0; i < tempList.count(); ++i) objectRemovedNotify(tempList.at(i), i);
        }
        else
        {
            qWarning() << "Can't clear an object whose index is out of bound";
        }
    }
    _Object* first() const
    {
        if(!_objects.isEmpty())
        {
            return _objects.first();
        }
        else
        {
            qWarning() << "The first element of an empty list doesn't exist !";
            return nullptr;
        }
    }

    _Object* last() const
    {
        if(!_objects.isEmpty())
        {
            return _objects.last();
        }
        else
        {
            qWarning() << "The last element of an empty list doesn't exist !";
            return nullptr;
        }
    }

    const QList<_Object*>& toList() const { return _objects; }

    // ──────── QML OVERRIDE API ──────────
public:
    void append(QJSValue value) override final
    {
        if(value.isQObject())
        {
            const auto castObject = qobject_cast<_Object*>(value.toQObject());
            if(castObject)
            {
                append(castObject);
            }
            else
            {
                qWarning() << templateClassName() << ": Fail to append" << value.toString() << ", item isn't a"
                           << _metaObj.className() << "derived class";
            }
        }
        else if(value.isArray())
        {
            QList<_Object*> listToAppend;
            for(int i = 0; i < value.property("length").toInt(); ++i)
            {
                const auto object = value.property(i);
                if(object.isQObject())
                {
                    const auto castObject = qobject_cast<_Object*>(object.toQObject());

                    if(castObject)
                    {
                        listToAppend.append(castObject);
                    }
                    else
                    {
                        qWarning() << templateClassName() << ": Fail to append " << object.toString()
                                   << ", item isn't a" << _metaObj.className() << "derived class";
                    }
                }
                else
                {
                    qWarning() << templateClassName() << ": Fail to append " << object.toString()
                               << ", item isn't QObject";
                }
            }
            if(!listToAppend.isEmpty())
                append(listToAppend);
        }
        else
        {
            qWarning() << templateClassName() << ": Fail to append" << value.toString()
                       << ", item isn't a QObject, an array of QObject";
        }
    }
    void prepend(QJSValue value) override final
    {
        if(value.isQObject())
        {
            const auto castObject = qobject_cast<_Object*>(value.toQObject());
            if(castObject)
            {
                prepend(castObject);
            }
            else
            {
                qWarning() << templateClassName() << ": Fail to prepend" << value.toString() << ", item isn't a"
                           << _metaObj.className() << "derived class";
            }
        }
        else if(value.isArray())
        {
            QList<_Object*> listToPrepend;
            for(int i = 0; i < value.property("length").toInt(); ++i)
            {
                const auto object = value.property(i);
                if(object.isQObject())
                {
                    const auto castObject = qobject_cast<_Object*>(object.toQObject());

                    if(castObject)
                    {
                        listToPrepend.append(castObject);
                    }
                    else
                    {
                        qWarning() << templateClassName() << ": Fail to prepend " << object.toString()
                                   << ", item isn't a" << _metaObj.className() << "derived class";
                    }
                }
                else
                {
                    qWarning() << templateClassName() << ": Fail to prepend " << object.toString()
                               << ", item isn't QObject";
                }
            }
            if(!listToPrepend.isEmpty())
                prepend(listToPrepend);
        }
        else
        {
            qWarning() << templateClassName() << ": Fail to prepend" << value.toString()
                       << ", item isn't a QObject, an array of QObject";
        }
    }

    void insert(int idx, QJSValue value) override final
    {
        if(value.isQObject())
        {
            const auto castObject = qobject_cast<_Object*>(value.toQObject());
            if(castObject)
            {
                insert(idx, castObject);
            }
            else
            {
                qWarning() << templateClassName() << ": Fail to insert" << value.toString() << ", item isn't a"
                           << _metaObj.className() << "derived class";
            }
        }
        else if(value.isArray())
        {
            QList<_Object*> listToInsert;
            for(int i = 0; i < value.property("length").toInt(); ++i)
            {
                const auto object = value.property(i);
                if(object.isQObject())
                {
                    const auto castObject = qobject_cast<_Object*>(object.toQObject());

                    if(castObject)
                    {
                        listToInsert.append(castObject);
                    }
                    else
                    {
                        qWarning() << templateClassName() << ": Fail to insert " << object.toString()
                                   << ", item isn't a" << _metaObj.className() << "derived class";
                    }
                }
                else
                {
                    qWarning() << templateClassName() << ": Fail to insert " << object.toString()
                               << ", item isn't QObject";
                }
            }
            if(!listToInsert.isEmpty())
                insert(idx, listToInsert);
        }
        else
        {
            qWarning() << templateClassName() << ": Fail to insert" << value.toString()
                       << ", item isn't a QObject, an array of QObject";
        }
    }

    void remove(QJSValue value) final
    {
        if(value.isNumber())
        {
            const auto index = int(value.toNumber());
            remove(index);
        }
        else if(value.isQObject())
        {
            const auto castObject = qobject_cast<_Object*>(value.toQObject());
            if(castObject)
            {
                remove(castObject);
            }
            else
            {
                qWarning() << templateClassName() << ": Fail to remove" << value.toString() << ", item isn't a"
                           << _metaObj.className() << "derived class";
            }
        }
        else if(value.isArray())
        {
            QList<_Object*> listToRemove;
            for(int i = 0; i < value.property("length").toInt(); ++i)
            {
                const auto object = value.property(i);
                if(object.isQObject())
                {
                    const auto castObject = qobject_cast<_Object*>(object.toQObject());

                    if(castObject)
                    {
                        listToRemove.append(castObject);
                    }
                    else
                    {
                        qWarning() << templateClassName() << ": Fail to insert " << object.toString()
                                   << ", item isn't a" << _metaObj.className() << "derived class";
                    }
                }
                else
                {
                    qWarning() << templateClassName() << ": Fail to insert " << object.toString()
                               << ", item isn't QObject";
                }
            }
            if(!listToRemove.isEmpty())
                remove(listToRemove);
        }
        else
        {
            qWarning() << templateClassName() << ": Fail to remove" << value.toString()
                       << ", item isn't a QObject, an array of QObject or a number";
        }
    }

    bool contains(QJSValue value) const final
    {
        if(value.isQObject())
        {
            const auto castObject = qobject_cast<_Object*>(value.toQObject());
            if(castObject)
            {
                return contains(castObject);
            }
            else
            {
                qWarning() << templateClassName() << ": Fail to get indexOf" << value.toString() << ", item isn't a"
                           << _metaObj.className() << "derived class";
            }
        }
        else
        {
            qWarning() << templateClassName() << ": Fail to get indexOf" << value.toString()
                       << ", item isn't a QObject";
        }
        return false;
    }

    int indexOf(QJSValue value) const override final
    {
        if(value.isQObject())
        {
            const auto castObject = qobject_cast<_Object*>(value.toQObject());
            if(castObject)
            {
                return indexOf(castObject);
            }
            else
            {
                qWarning() << templateClassName() << ": Fail to get indexOf" << value.toString() << ", item isn't a"
                           << _metaObj.className() << "derived class";
            }
        }
        else
        {
            qWarning() << templateClassName() << ": Fail to get indexOf" << value.toString()
                       << ", item isn't a QObject";
        }
        return -1;
    }

    /** \brief Move index to index-1 */
    void moveUp(const int index) override final
    {
        if(index > 0 && index < count())
        {
            move(index, index - 1);
        }
        else
        {
        qWarning() << "The index is the first of the list or index is out of bound";
        }
    }

    /** \brief Move index to index+1 */
    void moveDown(const int index) override final
    {
        if(count() &&  // There is a least one entry
            index >= 0 &&  // We can be from the first
            index < (count() - 1)  // To the last one minus 1
        )
        {
            move(index, index + 1);
        }
        else
        {
            qWarning() << "The index is the last of the list or index is out of bound";
        }
    }

    // ──────── DEFAULT CHILDREN ──────────
protected:
    void appendDefaultChild(QObject* child) override
    {
        const auto object = qobject_cast<_Object*>(child);
        if(object)
        {
            _defaultObjects.append(object);
            append(object);
        }
        else
        {
            qWarning() << templateClassName() << ": Fail to append default child " << child->objectName()
                       << ", that isn't a" << _metaObj.className() << "derived class";
        }
    }

    int defaultChildrenCount() override { return _defaultObjects.count(); }

    QObject* defaultChild(int index) override
    {
        if(_defaultObjects.count() < index)
            return nullptr;

        return _defaultObjects.at(index);
    }

    void clearDefaultChildren() override
    {
        remove(_defaultObjects);
        _defaultObjects.clear();
    }

    void replaceDefaultChild(int index, QObject* child) override
    {
        const auto object = qobject_cast<_Object*>(child);
        if(!object)
        {
            qWarning() << templateClassName() << ": Fail to append default child " << child->objectName()
                       << ", that isn't a" << _metaObj.className() << "derived class";
        }

        if(_defaultObjects.count() < index)
            return;

        const auto previousObject = _defaultObjects.at(index);
        const auto previousObjectIndex = indexOf(previousObject);
        if(previousObject)
            remove(previousObject);

        _defaultObjects.replace(index, object);

        if(child)
            insert(previousObjectIndex, object);
    }

    void removeLastChild() override
    {
        if(_defaultObjects.isEmpty())
            return;

        const auto objectToRemove = _defaultObjects.last();
        remove(objectToRemove);
        _defaultObjects.removeLast();
    }

    // ──────── OBSERVER API ──────────
private:
    void objectAboutToBeInsertedNotify(_Object* object, int index) { onObjectAboutToBeInserted(object, index); }
    void objectInsertedNotify(_Object* object, int index)
    {
        onObjectInserted(object, index);
        _callbacks.inserted(InsertedCallbackArgs(object, index));
        Q_EMIT objectInserted(object, index);
    }
    void objectAboutToBeMovedNotify(_Object* object, int src, int dest) { onObjectAboutToBeMoved(object, src, dest); }
    void objectMovedNotify(_Object* object, int src, int dest)
    {
        onObjectMoved(object, src, dest);
        _callbacks.moved(MovedCallbackArgs(object, src, dest));
        Q_EMIT objectMoved(object, src, dest);
    }
    void objectAboutToBeRemovedNotify(_Object* object, int index)
    {
        onObjectAboutToBeRemoved(object, index);
        _callbacks.removed(RemovedCallbackArgs(object, index));
        Q_EMIT objectRemoved(object, index);
    }
    void objectRemovedNotify(_Object* object, int index) { onObjectRemoved(object, index); }

    // ──────── CALLBACK API ──────────
public:
    struct _BaseCallbackArgs
    {
        explicit _BaseCallbackArgs(_Object* object) : object(object) {}

        _Object* object = nullptr;

        _Object* operator->() const { return object; }
        operator _Object*() const { return object; }
    };
    struct InsertedCallbackArgs : _BaseCallbackArgs
    {
        InsertedCallbackArgs(_Object* object, int index) : _BaseCallbackArgs(object), index(index) {}

        int index = -1;
    };
    using RemovedCallbackArgs = InsertedCallbackArgs;

    struct MovedCallbackArgs : _BaseCallbackArgs
    {
        MovedCallbackArgs(_Object* object, int from, int to) : _BaseCallbackArgs(object), from(from), to(to) {}

        int from = -1;
        int to = -1;
    };

    using InsertedCallbackList = eventpp::CallbackList<void(const InsertedCallbackArgs&)>;
    using RemovedCallbackList = eventpp::CallbackList<void(const RemovedCallbackArgs&)>;
    using MovedCallbackList = eventpp::CallbackList<void(const MovedCallbackArgs&)>;

    struct Callbacks
    {
        InsertedCallbackList inserted;
        RemovedCallbackList removed;
        MovedCallbackList moved;
    };

public:
    typename InsertedCallbackList::Handle onInserted(std::function<void(const InsertedCallbackArgs&)> callback)
    {
        return callback ? _callbacks.inserted.append(callback) : typename InsertedCallbackList::Handle();
    }
    typename RemovedCallbackList::Handle onRemoved(std::function<void(const RemovedCallbackArgs&)> callback)
    {
        return callback ? _callbacks.removed.append(callback) : typename RemovedCallbackList::Handle();
    }
    typename MovedCallbackList::Handle onMoved(std::function<void(const MovedCallbackArgs&)> callback)
    {
        return callback ? _callbacks.moved.append(callback) : typename MovedCallbackList::Handle();
    }

    void stopListenInsert(typename InsertedCallbackList::Handle h) { _callbacks.inserted.remove(h); }
    void stopListenRemove(typename RemovedCallbackList::Handle h) { _callbacks.removed.remove(h); }
    void stopListenMove(typename MovedCallbackList::Handle h) { _callbacks.moved.remove(h); }

protected:
    virtual void onObjectAboutToBeInserted(_Object* object, int index) {}
    virtual void onObjectInserted(_Object* object, int index) {}
    virtual void onObjectAboutToBeMoved(_Object* object, int src, int dest) {}
    virtual void onObjectMoved(_Object* object, int src, int dest) {}
    virtual void onObjectAboutToBeRemoved(_Object* object, int index) {}
    virtual void onObjectRemoved(_Object* object, int index) {}

    // ──────── ATTRIBUTES ──────────
private:
    int _count;
    QByteArray _displayRoleName;
    QMetaObject _metaObj;
    QMetaMethod _handler;
    QHash<int, QByteArray> _roles;
    QHash<int, int> _signalIdxToRole;
    QList<_Object*> _objects;
    Callbacks _callbacks;
    QList<_Object*> _defaultObjects;
};

}

#endif
