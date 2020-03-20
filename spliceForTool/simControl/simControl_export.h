
#ifndef SIMCONTROL_EXPORT_H
#define SIMCONTROL_EXPORT_H

#ifdef SIMCONTROL_STATIC_DEFINE
#  define SIMCONTROL_EXPORT
#  define SIMCONTROL_NO_EXPORT
#else
#  ifndef SIMCONTROL_EXPORT
#    ifdef simControl_EXPORTS
        /* We are building this library */
#      define SIMCONTROL_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define SIMCONTROL_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef SIMCONTROL_NO_EXPORT
#    define SIMCONTROL_NO_EXPORT 
#  endif
#endif

#ifndef SIMCONTROL_DEPRECATED
#  define SIMCONTROL_DEPRECATED __declspec(deprecated)
#endif

#ifndef SIMCONTROL_DEPRECATED_EXPORT
#  define SIMCONTROL_DEPRECATED_EXPORT SIMCONTROL_EXPORT SIMCONTROL_DEPRECATED
#endif

#ifndef SIMCONTROL_DEPRECATED_NO_EXPORT
#  define SIMCONTROL_DEPRECATED_NO_EXPORT SIMCONTROL_NO_EXPORT SIMCONTROL_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef SIMCONTROL_NO_DEPRECATED
#    define SIMCONTROL_NO_DEPRECATED
#  endif
#endif

#endif /* SIMCONTROL_EXPORT_H */
