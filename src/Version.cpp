// ─────────────────────────────────────────────────────────────
//                  INCLUDE
// ─────────────────────────────────────────────────────────────

// C Header

// C++ Header

// Qt Header
#include <QString>

// Dependencies Header

// Application Header
#include <ObjectListModel/Version.hpp>

// ─────────────────────────────────────────────────────────────
//                  DECLARATION
// ─────────────────────────────────────────────────────────────

OBJLISTMODEL_USING_NAMESPACE

// ─────────────────────────────────────────────────────────────
//                  FUNCTIONS
// ─────────────────────────────────────────────────────────────

uint32_t Version::getMajor()
{
    return OBJLISTMODEL_VERSION_MAJOR;
}

uint32_t Version::getMinor()
{
    return OBJLISTMODEL_VERSION_MINOR;
}

uint32_t Version::getPatch()
{
    return OBJLISTMODEL_VERSION_PATCH;
}

uint32_t Version::getTag()
{
    return OBJLISTMODEL_VERSION_TAG_HEX;
}

QString Version::getVersion()
{
    return QString::number(getMajor()) + "." +
        QString::number(getMinor()) + "." +
        QString::number(getPatch()) + "." +
        QString::number(getTag(), 16);
}
