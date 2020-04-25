#ifndef __QOLM_QOLMBASE_HPP__
#define __QOLM_QOLMBASE_HPP__

#include <QOlm/Export.hpp>

#include <QAbstractListModel>
#include <QJSValue>

namespace QOlm {

class QOLM_API_ QOlmBase : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(int length READ count NOTIFY countChanged)
    Q_PROPERTY(int size READ count NOTIFY countChanged)

    // ──────── CONSTRUCTOR ──────────
public:
    explicit QOlmBase(QObject* parent = nullptr);
    virtual ~QOlmBase() = default;

    // ──────── QML API ──────────
public Q_SLOTS:
    /**
     * \brief Return the count of element inside the list
     * From js, count is exposed as property so you simply need to do:
     * \code
     * const myModel = new MyModel()
     * const myModelCount = myModel.count
     * \endcode
     * This function is here to match Qt QList api.
     * \return the count of element inside the list
     */
    virtual int count() const = 0;

    /**
     * \brief Return the count of element inside the list
     * From js, count is exposed as property so you simply need to do:
     * \code
     * const myModel = new MyModel()
     * const myModelCount = myModel.size
     * \endcode
     * This function is here to match c++ stl api
     * \return the count of element inside the list
     */
    int size() const;

    /**
     * \brief Return the count of element inside the list
     * From js, count is exposed as property so you simply need to do:
     * \code
     * const myModel = new MyModel()
     * const myModelCount = myModel.length
     * \endcode
     * This function is here to match js stl api
     * \return the count of element inside the list
     */
    int length() const;

    /**
     * \brief The model is empty
     * This function is here to match c++ stl api.
     * \return true if the model is empty
     */
    bool empty() const;

    /**
     * \brief Get a pointer to the object contained at index
     *
     * \code
     * // ...
     * const myObject = myModel.getQtObject(8);
     * myObject.foo = 10
     * // ...
     * \endcode
     *
     * \param index Should be from 0 to (count-1)
     * \return Valid pointer if object is present, nullptr otherwise
     */
    virtual QObject* get(QJSValue index) const = 0;

    /**
     * \brief Get the index of a qtObject inside a model
     * This function is here to match js standard api
     *
     * \code
     * // ...
     * const index = getIndex.getQtObject(myObject);
     * // index == 8
     * // ...
     * \endcode
     *
     * \param object pointer to the qtObject
     * \return valid index from 0 to (count-1) if object is found, otherwise -1.
     */
    virtual int indexOf(QObject* object) const = 0;

    /**
     * \brief get if an object is present inside the model
     * This function match js standard api
     * \param object pointer to the object to look for
     * \return true if object is present in model
     */
    virtual bool contains(QObject* object) const = 0;

    /** \brief Get the role id of name, -1 if role not found */
    virtual int roleForName(const QByteArray& name) const = 0;

    /**
     * \brief Inserts value at the end of the list.
     * This is the same as list.insert(size(), value).
     * If this list is not shared, this operation is typically
     * very fast (amortized constant time),
     * because QList preallocates extra space on both sides of
     * its internal buffer to allow for fast growth at both ends of the list.
     * This function match qt api
     */
    virtual void append(QObject* object) = 0;

    /**
     * \brief Inserts value at the beginning of the list.
     * This is the same as list.insert(0, value).
     * If this list is not shared, this operation is typically very fast
     * (amortized constant time), because QList preallocates extra space
     * on both sides of its internal buffer to allow for fast growth at
     * both ends of the list.
     */
    virtual void prepend(QObject* item) = 0;

    /**
     * \brief Inserts value at index position i in the list.
     * If i <= 0, the value is prepended to the list.
     * If i >= size(), the value is appended to the list.
     */
    virtual void insert(int index, QObject* item) = 0;

    /**
     * \brief Can be called to remove a QObject* or directly using index
     */
    virtual void remove(QJSValue value) = 0;

    /**
     * \brief Removes all items from the list. It won't delete any object.
     * In this js this is the equivalent of myModel = [] or myModel.length = 0
     * Since it's not possible to implement such operator, you need to use the clear function.
     */
    virtual void clear(void) = 0;

    /**
     * \brief Moves the item at index position from to index position to.
     * This is the same as insert(to, takeAt(from)).
     * This function assumes that both from and to are at least 0 but less than size().
     * To avoid failure, test that both from and to are at least 0 and less than size().
     */
    virtual void move(int index, int to) = 0;
    /**
     * \brief swap Content of index with previous object
     */
    virtual void moveDown(const int index) = 0;
    /**
     * \brief swap Content of index with next object
     */
    virtual void moveUp(const int index) = 0;

    void moveNext(const int index);
    void movePrevious(const int index);

    // ──────── INTERNAL CALLBACK ──────────
protected Q_SLOTS:
    virtual void onItemPropertyChanged() = 0;

    // ──────── SIGNALS ──────────
Q_SIGNALS:
    void countChanged();

    void objectInserted(QObject* object, int index);
    void objectRemoved(QObject* object, int index);
    void objectMoved(QObject* object, int from, int to);
};

}

#endif
