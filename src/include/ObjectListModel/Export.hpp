#ifndef __OBJLISTMODEL_EXPORT_HPP__
#define __OBJLISTMODEL_EXPORT_HPP__

// ─────────────────────────────────────────────────────────────
//                  DECLARATION
// ─────────────────────────────────────────────────────────────

#ifdef WIN32
    #ifdef OBJLISTMODEL_DLL_EXPORT  // Shared build
        #define OBJLISTMODEL_API_ __declspec(dllexport)
    #elif OBJLISTMODEL_STATIC       // No decoration when building staticlly
        #define OBJLISTMODEL_API_
    #else                       // Link to lib
        #define OBJLISTMODEL_API_ __declspec(dllimport)
    #endif
#else
    #define OBJLISTMODEL_API_
#endif // WIN32

#ifdef OBJLISTMODEL_USE_NAMESPACE

    #ifndef OBJLISTMODEL_NAMESPACE
        #define OBJLISTMODEL_NAMESPACE Olm
    #endif // ifndef OBJLISTMODEL_NAMESPACE

    #define OBJLISTMODEL_NAMESPACE_START namespace OBJLISTMODEL_NAMESPACE {
    #define OBJLISTMODEL_NAMESPACE_END }
    #define OBJLISTMODEL_USING_NAMESPACE using namespace OBJLISTMODEL_NAMESPACE;

#else // OBJLISTMODEL_USE_NAMESPACE

    #undef OBJLISTMODEL_NAMESPACE
    #define OBJLISTMODEL_NAMESPACE
    #define OBJLISTMODEL_NAMESPACE_START
    #define OBJLISTMODEL_NAMESPACE_END
    #define OBJLISTMODEL_USING_NAMESPACE

#endif // OBJLISTMODEL_USE_NAMESPACE

#endif // __OBJLISTMODEL_EXPORT_HPP__
