
#ifndef CSSIMLOG_EXPORT_H
#define CSSIMLOG_EXPORT_H

#ifdef CSSIMLOG_STATIC_DEFINE
#  define CSSIMLOG_EXPORT
#  define CSSIMLOG_NO_EXPORT
#else
#  ifndef CSSIMLOG_EXPORT
#    ifdef cssimlog_EXPORTS
        /* We are building this library */
#      define CSSIMLOG_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define CSSIMLOG_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef CSSIMLOG_NO_EXPORT
#    define CSSIMLOG_NO_EXPORT 
#  endif
#endif

#ifndef CSSIMLOG_DEPRECATED
#  define CSSIMLOG_DEPRECATED __declspec(deprecated)
#endif

#ifndef CSSIMLOG_DEPRECATED_EXPORT
#  define CSSIMLOG_DEPRECATED_EXPORT CSSIMLOG_EXPORT CSSIMLOG_DEPRECATED
#endif

#ifndef CSSIMLOG_DEPRECATED_NO_EXPORT
#  define CSSIMLOG_DEPRECATED_NO_EXPORT CSSIMLOG_NO_EXPORT CSSIMLOG_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef CSSIMLOG_NO_DEPRECATED
#    define CSSIMLOG_NO_DEPRECATED
#  endif
#endif

#endif /* CSSIMLOG_EXPORT_H */
