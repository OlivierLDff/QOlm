#ifndef __OBJLISTMODEL_COMMON_HPP__
#define __OBJLISTMODEL_COMMON_HPP__

// ─────────────────────────────────────────────────────────────
//                  INCLUDE
// ─────────────────────────────────────────────────────────────

// C Header
#include <stdint.h>

// C++ Header

// Qt Header
#include <QString>

// Dependencies Header

// Application Header
#include <ObjectListModel/Export.hpp>

// ─────────────────────────────────────────────────────────────
//                  DECLARATION
// ─────────────────────────────────────────────────────────────

OBJLISTMODEL_NAMESPACE_START

class OBJLISTMODEL_API_ Version
{
public:
    /** Library Major Version */
    static uint32_t getMajor();
    /** Library Minor Version */
    static uint32_t getMinor();
    /** Library Patch Version */
    static uint32_t getPatch();
    /** Library Tag Version */
    static uint32_t getTag();
    /** Library Version as major.minor.patch.tag */
    static QString getVersion();
};

OBJLISTMODEL_NAMESPACE_END

#endif
