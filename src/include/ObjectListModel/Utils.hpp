#ifndef __OBJLISTMODEL_UTILS_HPP__
#define __OBJLISTMODEL_UTILS_HPP__

// ─────────────────────────────────────────────────────────────
//                  INCLUDE
// ─────────────────────────────────────────────────────────────

// C Header

// C++ Header

// Qt Header

// Dependencies Header

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
class OBJLISTMODEL_API_ Utils
{
public:
	/**
	 * Register type to the qml engines
	 * Registered types are:
	 * -
	 */
	static void registerTypes(const char* uri, const quint8 major, const quint8 minor);
};

OBJLISTMODEL_NAMESPACE_END

#endif
