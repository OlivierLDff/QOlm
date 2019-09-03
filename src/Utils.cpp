// ─────────────────────────────────────────────────────────────
//                  INCLUDE
// ─────────────────────────────────────────────────────────────

// C Header

// C++ Header

// Qt Header
#include <QCoreApplication> // Call register type at startup when loaded as a dynamic library
#include <QLoggingCategory> // Logging support

// Dependancies Header

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

static void registerTypes()
{
    // Controller
    //qCDebug(OBJLISTMODEL_REGISTER_LOG_CAT, "Register %s.Controller %d.%d to QML", *_uri, _major, _minor);
    //OBJLISTMODEL_NAMESPACE::Controller::registerToQml(*_uri, _major, _minor);
}

static void registerTypes(const char* uri, const quint8 major, const quint8 minor)
{
    if(uri)
        _uri = &uri;
    _major = major;
    _minor = minor;
    registerTypes();
}

Q_COREAPP_STARTUP_FUNCTION(registerTypes)

OBJLISTMODEL_USING_NAMESPACE;

void Utils::registerTypes(const char* uri, const quint8 major, const quint8 minor)
{
	::registerTypes(uri, major, minor);
}
