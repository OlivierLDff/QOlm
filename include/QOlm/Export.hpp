#ifndef __QOLM_EXPORT_HPP__
#define __QOLM_EXPORT_HPP__

#ifdef WIN32
#    ifdef QOLM_DLL_EXPORT
#        define QOLM_API_ __declspec(dllexport)
#    elif QOLM_STATIC
#        define QOLM_API_
#    else
#        define QOLM_API_ __declspec(dllimport)
#    endif
#else
#    define QOLM_API_
#endif

#endif
