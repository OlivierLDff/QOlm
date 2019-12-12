// ─────────────────────────────────────────────────────────────
//                  INCLUDE
// ─────────────────────────────────────────────────────────────

// C Header

// C++ Header

// Qt Header
#include <QCoreApplication> // Call register type at startup when loaded as a dynamic library
#include <QLoggingCategory> // Logging support

// Dependencies Header

// Application Header
#include <ObjectListModel/Utils.hpp>

// ─────────────────────────────────────────────────────────────
//                  DECLARATION
// ─────────────────────────────────────────────────────────────

Q_LOGGING_CATEGORY(OBJLISTMODEL_REGISTER_LOG_CAT, "objectListModel.utils")

// ─────────────────────────────────────────────────────────────
//                  FUNCTIONS
// ─────────────────────────────────────────────────────────────

static const char* _defaultUri = "ObjectListModel";
static const char** _uri = &_defaultUri;
static quint8 _major = 1;
static quint8 _minor = 0;

static void ObjListModel_registerTypes()
{
}

static void ObjListModel_registerTypes(const char* uri, const quint8 major, const quint8 minor)
{
    if(uri)
        _uri = &uri;
    _major = major;
    _minor = minor;
    ObjListModel_registerTypes();
}

#ifndef OBJLISTMODEL_STATIC
Q_COREAPP_STARTUP_FUNCTION(ObjListModel_registerTypes)
#endif

OBJLISTMODEL_USING_NAMESPACE;

void Utils::registerTypes(const char* uri, const quint8 major, const quint8 minor)
{
	::ObjListModel_registerTypes(uri, major, minor);
}
