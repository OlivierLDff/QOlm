#ifndef __QOLM_EXPORT_HPP__
#define __QOLM_EXPORT_HPP__

#ifdef WIN32
    #ifdef QOLM_DLL_EXPORT  // Shared build
        #define QOLM_API_ __declspec(dllexport)
    #elif QOLM_STATIC       // No decoration when building staticlly
        #define QOLM_API_
    #else                  // Link to lib
        #define QOLM_API_ __declspec(dllimport)
    #endif
#else
    #define QOLM_API_
#endif // WIN32

#endif