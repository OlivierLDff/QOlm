#ifndef __OBJLISTMODEL_TEMPLATE_HPP__
#define __OBJLISTMODEL_TEMPLATE_HPP__

// ─────────────────────────────────────────────────────────────
//                  INCLUDE
// ─────────────────────────────────────────────────────────────

// C Header

// C++ Header

// Qt Header
#include <QObject>

// Dependancies Header

// Application Header
#include <ObjectListModel/Export.hpp>

// ─────────────────────────────────────────────────────────────
//                  DECLARATION
// ─────────────────────────────────────────────────────────────

OBJLISTMODEL_NAMESPACE_START

// ─────────────────────────────────────────────────────────────
//                  CLASS
// ─────────────────────────────────────────────────────────────

/**
 */
class OBJLISTMODEL_API_ ITemplate
{
public:
    virtual ~Template() = default;
};

/**
 */
class OBJLISTMODEL_API_ BaseTemplate: public QObject, public ITemplate
{
    Q_OBJECT

public:
    BaseTemplate(QObject* parent = nullptr);
    virtual ~BaseTemplate() = default;
};

/**
 */
class OBJLISTMODEL_API_ Template: public BaseTemplate
{
    Q_OBJECT

public:
    Template(QObject* parent = nullptr);
    virtual ~Template() = default;
};


OBJLISTMODEL_NAMESPACE_END

#endif
