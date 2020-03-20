
#ifndef CSDDS_SERVICE_EXPORT_H
#define CSDDS_SERVICE_EXPORT_H

#ifdef CSDDS_SERVICE_STATIC_DEFINE
#  define CSDDS_SERVICE_EXPORT
#  define CSDDS_SERVICE_NO_EXPORT
#else
#  ifndef CSDDS_SERVICE_EXPORT
#    ifdef csdds_service_EXPORTS
        /* We are building this library */
#      define CSDDS_SERVICE_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define CSDDS_SERVICE_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef CSDDS_SERVICE_NO_EXPORT
#    define CSDDS_SERVICE_NO_EXPORT 
#  endif
#endif

#ifndef CSDDS_SERVICE_DEPRECATED
#  define CSDDS_SERVICE_DEPRECATED __declspec(deprecated)
#endif

#ifndef CSDDS_SERVICE_DEPRECATED_EXPORT
#  define CSDDS_SERVICE_DEPRECATED_EXPORT CSDDS_SERVICE_EXPORT CSDDS_SERVICE_DEPRECATED
#endif

#ifndef CSDDS_SERVICE_DEPRECATED_NO_EXPORT
#  define CSDDS_SERVICE_DEPRECATED_NO_EXPORT CSDDS_SERVICE_NO_EXPORT CSDDS_SERVICE_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef CSDDS_SERVICE_NO_DEPRECATED
#    define CSDDS_SERVICE_NO_DEPRECATED
#  endif
#endif

#endif /* CSDDS_SERVICE_EXPORT_H */
