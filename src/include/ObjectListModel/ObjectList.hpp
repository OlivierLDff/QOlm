#ifndef __OBJECTLISTMODEL_OBJECTLIST_HPP__
#define __OBJECTLISTMODEL_OBJECTLIST_HPP__

#include <QAbstractListModel>
#include <QByteArray>
#include <QChar>
#include <QDebug>
#include <QHash>
#include <QList>
#include <QMetaMethod>
#include <QMetaObject>
#include <QMetaProperty>
#include <QObject>
#include <QString>
#include <QStringBuilder>
#include <QVariant>
#include <QVector>

#include <ObjectListModel/Export.hpp>

OBJLISTMODEL_NAMESPACE_START

// custom foreach for QList, which uses no copy and check pointer non-null
#define OBJLISTMODEL_OBJECTLIST_FOREACH_PTR_IN_QLIST(_type_, _var_, _list_) \
    for(typename QList<_type_ *>::const_iterator it = _list_.constBegin(); it != _list_.constEnd(); ++it) \
        if(_type_ * _var_ =(*it))

OBJLISTMODEL_API_ class ObjectListBase : public QAbstractListModel
{ // abstract Qt base class
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(int length READ count NOTIFY countChanged)

public:
    explicit ObjectListBase(QObject* parent = Q_NULLPTR) : QAbstractListModel(parent) { }

public slots: // virtual methods API for QML
    /** Returns the number of items in the list.
     * in js model.size() */
    virtual int size(void) const = 0;
    /** This function is provided for STL compatibility.
     * It is equivalent to isEmpty() and returns true if the list is empty.
     * model.count or model.length in js will work */
    virtual int count(void) const = 0;
    /** Returns true if the list contains no items; otherwise returns false. */
    virtual bool isEmpty(void) const = 0;
    /** Returns true if the list contains an occurrence of value; otherwise returns false.
     * This function requires the value type to have an implementation of operator==(). */
    virtual bool contains(QObject* item) const = 0;
    /** Returns the index position of the first occurrence of value in the list,
     * searching forward from index position from. Returns -1 if no item matched.
     * \param item ptr the item you want the index of */
    virtual int indexOf(QObject* item) const = 0;
    /** Get the role id of name, -1 if role not found */
    virtual int roleForName(const QByteArray & name) const = 0;
    /** Removes all items from the list. It won't delete any object*/
    virtual void clear(void) = 0;
    /** Inserts value at the end of the list.
     * This is the same as list.insert(size(), value).
     * If this list is not shared, this operation is typically
     * very fast(amortized constant time),
     * because QList preallocates extra space on both sides of
     * its internal buffer to allow for fast growth at both ends of the list.
     */
    virtual void append(QObject* item) = 0;
    /** Inserts value at the beginning of the list.
     * This is the same as list.insert(0, value).
     * If this list is not shared, this operation is typically very fast
     *(amortized constant time), because QList preallocates extra space
     * on both sides of its internal buffer to allow for fast growth at
     * both ends of the list.
     */
    virtual void prepend(QObject* item) = 0;
    /** Inserts value at index position i in the list.
     * If i <= 0, the value is prepended to the list.
     * If i >= size(), the value is appended to the list. */
    virtual void insert(int idx, QObject* item) = 0;
    /** Moves the item at index position from to index position to.
     * This is the same as insert(to, takeAt(from)).
     * This function assumes that both from and to are at least 0 but less than size().
     * To avoid failure, test that both from and to are at least 0 and less than size().
     */
    virtual void move(int idx, int pos) = 0;
    virtual void moveDown(const int row) = 0;
    virtual void moveUp(const int row) = 0;
    virtual void remove(QObject* item) = 0;
    /** Removes the item at index position i.
     *i must be a valid index position in the list(i.e., 0 <= i < size()). */
    virtual void remove(int idx) = 0;
    virtual QObject* get(int idx) const = 0;
    virtual QObject* get(const QString & uid) const = 0;
    virtual QObject* getFirst(void) const = 0;
    virtual QObject* getLast(void) const = 0;

protected slots: // internal callback
    virtual void onItemPropertyChanged(void) = 0;

signals: // notifier
    /** Emitted when count changed(ie removed or inserted item) */
    void countChanged(void);
signals:
    /** Emitted when an item is about to be inserted */
    void itemAboutToBeInserted(QObject* item, int row);
    /** Emitted after an item is in the list*/
    void itemInserted(QObject* item, int row);
    /** Emitted when an item is about to be moved */
    void itemAboutToBeMoved(QObject* item, int src, int dest);
    /** Emitted after an item have been moved */
    void itemMoved(QObject* item, int src, int dest);
    /** Emitted when an item is about to be removed */
    void itemAboutToBeRemoved(QObject* item, int row);
    /** Emitted when an item is about to be removed */
    void itemRemoved(QObject* item, int row);
};

OBJLISTMODEL_API_
template<class ItemType>
class ObjectList : public ObjectListBase
{
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
     * @param[in]  uidRole       The uid role is used to uniquely identify the object in the list. In the library ObjectList, uid can be whatever but here we want to use the network local id in the object as uid. Because, the uid uniqueness is provided by the NetworkObject backend.
     */
    explicit ObjectList(QObject* parent = Q_NULLPTR,
        const QList<QByteArray> & exposedRoles = {},
        const QByteArray & displayRole = {},
        const QByteArray& uidRole = {})
        : ObjectListBase(parent)
        , _count(0)
        , _uidRoleName(uidRole)
        , _dispRoleName(displayRole)
        , _metaObj(ItemType::staticMetaObject)
    {
        // Keep a track of black list rolename that are not compatible with Qml, they should never be used
        static QSet<QByteArray> roleNamesBlacklist;
        if(roleNamesBlacklist.isEmpty())
        {
            roleNamesBlacklist << QByteArrayLiteral("id")
                               << QByteArrayLiteral("index")
                               << QByteArrayLiteral("class")
                               << QByteArrayLiteral("model")
                               << QByteArrayLiteral("modelData");
        }

        // Set handler that handle every property changed
        static const char * HANDLER = "onItemPropertyChanged()";
        _handler = metaObject()->method(metaObject()->indexOfMethod(HANDLER));

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
        for(int propertyIdx = 0, role =(baseRole() +1); propertyIdx < len; propertyIdx++, role++)
        {
            QMetaProperty metaProp = _metaObj.property(propertyIdx);
            const QByteArray propName = QByteArray(metaProp.name());
            // Only expose the property as a role if:
            // - It isn't blacklisted(id, index, class, model, modelData)
            // - When exposedRoles is empty we expose every property
            // - When exposedRoles isn't empty we only expose the property asked by the user
            if(!roleNamesBlacklist.contains(propName) &&
               (exposedRoles.size() == 0 || exposedRoles.contains(propName)))
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
                static const QByteArray CLASS_NAME =(QByteArrayLiteral("ObjectList<") % _metaObj.className() % '>');
                qWarning() << "Can't have" << propName << "as a role name in" << qPrintable(CLASS_NAME) << ", because it's a blacklisted keywork in QML!. "
                "Please don't use any of the following words when declaring your Q_PROPERTY:(id, index, class, model, modelData)";
            }
        }
    }
    bool setData(const QModelIndex & index, const QVariant & value, int role) Q_DECL_FINAL
    {
        bool ret = false;
        ItemType* item = at(index.row());
        const QByteArray rolename =(role != Qt::DisplayRole ? _roles.value(role, emptyBA()) : _dispRoleName);
        if(item != Q_NULLPTR && role != baseRole() && !rolename.isEmpty())
        {
            ret = item->setProperty(rolename, value);
        }
        return ret;
    }
    QVariant data(const QModelIndex & index, int role) const Q_DECL_FINAL
    {
        QVariant ret;
        ItemType* item = at(index.row());
        const QByteArray rolename =(role != Qt::DisplayRole ? _roles.value(role, emptyBA()) : _dispRoleName);
        if(item != Q_NULLPTR && !rolename.isEmpty())
        {
            ret.setValue(role != baseRole() ? item->property(rolename) : QVariant::fromValue(static_cast<QObject*>(item)));
        }
        return ret;
    }
    QHash<int, QByteArray> roleNames(void) const Q_DECL_FINAL
    {
        return _roles;
    }
    typedef typename QList<ItemType*>::const_iterator const_iterator;
    const_iterator begin(void) const {
        return _items.begin();
    }
    const_iterator end(void) const {
        return _items.end();
    }
    const_iterator constBegin(void) const {
        return _items.constBegin();
    }
    const_iterator constEnd(void) const {
        return _items.constEnd();
    }

public: // C++ API
    ItemType* at(int idx) const {
        ItemType* ret = Q_NULLPTR;
        if(idx >= 0 && idx < _items.size())
        {
            ret = _items.value(idx);
        }
        return ret;
    }
    ItemType* getByUid(const QString & uid) const {
        return(!_indexByUid.isEmpty() ? _indexByUid.value(uid, Q_NULLPTR) : Q_NULLPTR);
    }
    int roleForName(const QByteArray & name) const Q_DECL_FINAL
    {
        return _roles.key(name, -1);
    }
    int count(void) const Q_DECL_FINAL
    {
        return _count;
    }
    int size(void) const Q_DECL_FINAL
    {
        return _count;
    }
    bool isEmpty(void) const Q_DECL_FINAL
    {
        return _items.isEmpty();
    }
    bool contains(ItemType* item) const {
        return _items.contains(item);
    }
    int indexOf(ItemType* item) const {
        return _items.indexOf(item);
    }
    void clear(void) Q_DECL_FINAL
    {
        if(!_items.isEmpty())
        {
            QList<ItemType*> tempList;
            for(int i = 0; i < _items.count(); ++i)
                itemAboutToBeRemoved(_items.at(i), i);
            beginRemoveRows(noParent(), 0, _items.count() -1);
            OBJLISTMODEL_OBJECTLIST_FOREACH_PTR_IN_QLIST(ItemType, item, _items)
            {
                dereferenceItem(item);
                tempList.append(item);
            }
            _items.clear();
            updateCounter();
            endRemoveRows();
            for(int i = 0; i < tempList.count(); ++i)
                itemRemoved(tempList.at(i), i);
        }
    }
    void append(ItemType* item)
    {
        if(item != Q_NULLPTR)
        {
            const int pos = _items.count();
            itemAboutToBeInserted(item, pos);
            beginInsertRows(noParent(), pos, pos);
            _items.append(item);
            referenceItem(item);
            updateCounter();
            endInsertRows();
            itemInserted(item, pos);
        }
    }
    void prepend(ItemType* item)
    {
        if(item != Q_NULLPTR)
        {
            itemAboutToBeInserted(item, 0);
            beginInsertRows(noParent(), 0, 0);
            _items.prepend(item);
            referenceItem(item);
            updateCounter();
            endInsertRows();
            itemInserted(item, 0);
        }
    }
    void insert(int idx, ItemType* item)
    {
        if(item != Q_NULLPTR)
        {
            itemAboutToBeInserted(item, idx);
            beginInsertRows(noParent(), idx, idx);
            _items.insert(idx, item);
            referenceItem(item);
            updateCounter();
            endInsertRows();
            itemInserted(item, idx);
        }
    }
    void append(const QList<ItemType*> & itemList)
    {
        if(!itemList.isEmpty())
        {
            const int pos = _items.count();
            for(int i = 0; i < itemList.count(); ++i)
                itemAboutToBeInserted(itemList.at(i), i + pos);

            beginInsertRows(noParent(), pos, pos + itemList.count() -1);
            _items.reserve(_items.count() + itemList.count());
            _items.append(itemList);
            OBJLISTMODEL_OBJECTLIST_FOREACH_PTR_IN_QLIST(ItemType, item, itemList)
            {
                referenceItem(item);
            }
            updateCounter();
            endInsertRows();
            for(int i = 0; i < itemList.count(); ++i)
                itemInserted(itemList.at(i), i + pos);
        }
    }
    void prepend(const QList<ItemType*> & itemList)
    {
        if(!itemList.isEmpty())
        {
            for(int i = 0; i < itemList.count(); ++i)
                itemAboutToBeInserted(itemList.at(i), i);
            beginInsertRows(noParent(), 0, itemList.count() -1);
            _items.reserve(_items.count() + itemList.count());
            int offset = 0;
            OBJLISTMODEL_OBJECTLIST_FOREACH_PTR_IN_QLIST(ItemType, item, itemList)
            {
                _items.insert(offset, item);
                referenceItem(item);
                offset++;
            }
            updateCounter();
            endInsertRows();
            for(int i = 0; i < itemList.count(); ++i)
                itemInserted(itemList.at(i), i);
        }
    }
    void insert(int idx, const QList<ItemType*> & itemList)
    {
        if(!itemList.isEmpty())
        {
            for(int i = 0; i < itemList.count(); ++i)
                itemAboutToBeInserted(itemList.at(i), i + idx);
            beginInsertRows(noParent(), idx, idx + itemList.count() -1);
            _items.reserve(_items.count() + itemList.count());
            int offset = 0;
            OBJLISTMODEL_OBJECTLIST_FOREACH_PTR_IN_QLIST(ItemType, item, itemList)
            {
                _items.insert(idx + offset, item);
                referenceItem(item);
                offset++;
            }
            updateCounter();
            endInsertRows();
            for(int i = 0; i < itemList.count(); ++i)
                itemInserted(itemList.at(i), i + idx);
        }
    }
    void move(int idx, int pos) Q_DECL_FINAL
    {
        if(idx != pos && idx >=0 && pos>=0 && idx < _items.size() && pos < _items.size())
        {
            itemAboutToBeMoved(_items.at(idx), idx, pos);
            beginMoveRows(noParent(), idx, idx, noParent(), (idx < pos ? pos +1 : pos));
            _items.move(idx, pos);
            endMoveRows();
            itemMoved(_items.at(idx), idx, pos);
        }
    }
    void remove(ItemType* item)
    {
        if(item != Q_NULLPTR)
        {
            const int idx = _items.indexOf(item);
            remove(idx);
        }
    }
    void remove(int idx) Q_DECL_FINAL
    {
        if(idx >= 0 && idx < _items.size())
        {
            itemAboutToBeRemoved(_items.at(idx), idx);
            beginRemoveRows(noParent(), idx, idx);
            ItemType* item = _items.takeAt(idx);
            dereferenceItem(item);
            updateCounter();
            endRemoveRows();
            itemRemoved(item, idx);
        }
    }
    ItemType* first(void) const {
        return _items.first();
    }
    ItemType* last(void) const {
        return _items.last();
    }
    const QList<ItemType*> & toList(void) const {
        return _items;
    }

public: // QML slots implementation
    void append(QObject* item) Q_DECL_FINAL
    {
        append(qobject_cast<ItemType*>(item));
    }
    void prepend(QObject* item) Q_DECL_FINAL
    {
        prepend(qobject_cast<ItemType*>(item));
    }
    void insert(int idx, QObject* item) Q_DECL_FINAL
    {
        insert(idx, qobject_cast<ItemType*>(item));
    }
    void remove(QObject* item) Q_DECL_FINAL
    {
        remove(qobject_cast<ItemType*>(item));
    }
    bool contains(QObject* item) const Q_DECL_FINAL
    {
        return contains(qobject_cast<ItemType*>(item));
    }
    int indexOf(QObject* item) const Q_DECL_FINAL
    {
        return indexOf(qobject_cast<ItemType*>(item));
    }
    int indexOf(const QString & uid) const {
        return indexOf(get(uid));
    }
    QObject* get(int idx) const Q_DECL_FINAL
    {
        return static_cast<QObject*>(at(idx));
    }
    QObject* get(const QString & uid) const Q_DECL_FINAL
    {
        return static_cast<QObject*>(getByUid(uid));
    }
    QObject* getFirst(void) const Q_DECL_FINAL
    {
        return static_cast<QObject*>(first());
    }
    QObject* getLast(void) const Q_DECL_FINAL
    {
        return static_cast<QObject*>(last());
    }

protected: // internal stuff
    static const QString & emptyStr(void)
    {
        static const QString ret = QStringLiteral("");
        return ret;
    }
    static const QByteArray & emptyBA(void)
    {
        static const QByteArray ret = QByteArrayLiteral("");
        return ret;
    }
    static const QModelIndex & noParent(void)
    {
        static const QModelIndex ret = QModelIndex();
        return ret;
    }
    static const int & baseRole(void)
    {
        static const int ret = Qt::UserRole;
        return ret;
    }
    int rowCount(const QModelIndex & parent = QModelIndex()) const Q_DECL_FINAL
    {
        return(!parent.isValid() ? _items.count() : 0);
    }
    void referenceItem(ItemType* item)
    {
        if(item != Q_NULLPTR)
        {
            if(!item->parent())
                item->setParent(this);

            for(QHash<int, int>::const_iterator it = _signalIdxToRole.constBegin(); it != _signalIdxToRole.constEnd(); ++it)
                connect(item, item->metaObject()->method(it.key()), this, _handler, Qt::UniqueConnection);

            if(!_uidRoleName.isEmpty())
            {
                const QString key = _indexByUid.key(item, emptyStr());

                if(!key.isEmpty())
                    _indexByUid.remove(key);

                const QString value = item->property(_uidRoleName).toString();

                if(!value.isEmpty())
                    _indexByUid.insert(value, item);
            }
        }
    }
    void dereferenceItem(ItemType* item)
    {
        if(item != Q_NULLPTR)
        {
            disconnect(this, Q_NULLPTR, item, Q_NULLPTR);
            disconnect(item, Q_NULLPTR, this, Q_NULLPTR);
            if(!_uidRoleName.isEmpty())
            {
                const QString key = _indexByUid.key(item, emptyStr());
                if(!key.isEmpty())
                    _indexByUid.remove(key);
            }
            if(item->parent() == this)
            { // FIXME : maybe that's not the best way to test ownership ?
                item->deleteLater();
            }
        }
    }
    void onItemPropertyChanged(void) Q_DECL_FINAL
    {
        ItemType* item = qobject_cast<ItemType*>(sender());
        const int row = _items.indexOf(item);
        const int sig = senderSignalIndex();
        const int role = _signalIdxToRole.value(sig, -1);
        if(row >= 0 && role >= 0)
        {
            const QModelIndex index = QAbstractListModel::index(row, 0, noParent());
            QVector<int> rolesList;
            rolesList.append(role);

            if(_roles.value(role) == _dispRoleName)
                rolesList.append(Qt::DisplayRole);

            emit dataChanged(index, index, rolesList);
        }
        if(!_uidRoleName.isEmpty())
        {
            const QByteArray roleName = _roles.value(role, emptyBA());
            if(!roleName.isEmpty() && roleName == _uidRoleName)
            {
                const QString key = _indexByUid.key(item, emptyStr());
                if(!key.isEmpty())
                    _indexByUid.remove(key);

                const QString value = item->property(_uidRoleName).toString();
                if(!value.isEmpty())
                    _indexByUid.insert(value, item);
            }
        }
    }
    inline void updateCounter(void)
    {
        if(_count != _items.count())
        {
            _count = _items.count();
            emit countChanged();
        }
    }

private:
    void itemAboutToBeInserted(ItemType* item, int row)
    { onItemAboutToBeInserted(item, row); _onItemAboutToBeInserted(item, row); }
    void itemInserted(ItemType* item, int row)
    { onItemInserted(item, row); _onItemInserted(item, row); }
    void itemAboutToBeMoved(ItemType* item, int src, int dest)
    { onItemAboutToBeMoved(item, src, dest); _onItemAboutToBeMoved(item, src, dest); }
    void itemMoved(ItemType* item, int src, int dest)
    { onItemMoved(item, src, dest); _onItemMoved(item, src, dest); }
    void itemAboutToBeRemoved(ItemType* item, int row)
    { onItemAboutToBeRemoved(item, row); _onItemAboutToBeRemoved(item, row); }
    void itemRemoved(ItemType* item, int row)
    { onItemRemoved(item, row); _onItemRemoved(item, row); }

protected:
    virtual void onItemAboutToBeInserted(ItemType* item, int row) { }
    virtual void onItemInserted(ItemType* item, int row) { }
    virtual void onItemAboutToBeMoved(ItemType* item, int src, int dest) { }
    virtual void onItemMoved(ItemType* item, int src, int dest) { }
    virtual void onItemAboutToBeRemoved(ItemType* item, int row) { }
    virtual void onItemRemoved(ItemType* item, int row) { }

private:
    void _onItemAboutToBeInserted(ItemType* item, int row)
    { emit ObjectListBase::itemAboutToBeInserted(item, row); }
    void _onItemInserted(ItemType* item, int row)
    { emit ObjectListBase::itemInserted(item, row); }
    void _onItemAboutToBeMoved(ItemType* item, int src, int dest)
    { emit ObjectListBase::itemAboutToBeMoved(item, src, dest); }
    void _onItemMoved(ItemType* item, int src, int dest)
    { emit ObjectListBase::itemMoved(item, src, dest); }
    void _onItemAboutToBeRemoved(ItemType* item, int row)
    { emit ObjectListBase::itemAboutToBeRemoved(item, row); }
    void _onItemRemoved(ItemType* item, int row)
    { emit ObjectListBase::itemRemoved(item, row); }

    /** Move row to row-1 */
    void moveUp(const int row) override final
    {
        if(row > 0 && row < count())
            move(row, row - 1);
    }

    /** Move row to row+1 */
    void moveDown(const int row) override final
    {
        if(count() && // There is a least one entry
            row >= 0 && // We can be from the first
            row <(count() - 1) // To the last one minus 1
            )
            return moveUp(row + 1);
    }

private: // data members
    int                        _count;
    QByteArray                 _uidRoleName;
    QByteArray                 _dispRoleName;
    QMetaObject                _metaObj;
    QMetaMethod                _handler;
    QHash<int, QByteArray>     _roles;
    QHash<int, int>            _signalIdxToRole;
    QList<ItemType*>           _items;
    QHash<QString, ItemType*>  _indexByUid;
};

/**
 * \internal
 */
class _Test_TestObj : public QObject
{
    Q_OBJECT
public:
    _Test_TestObj(QObject* parent = nullptr) : QObject(parent) { }
};

/**
 * \internal
 */
class _Test_Objlist : public ObjectList<_Test_TestObj>
{
    Q_OBJECT
};

OBJLISTMODEL_NAMESPACE_END

#endif // __OBJECTLISTMODEL_OBJECTLIST_HPP__
