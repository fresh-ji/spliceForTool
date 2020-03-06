
#ifndef CSSIMLOG_EXPORT_H
#define CSSIMLOG_EXPORT_H

#ifdef CSSIMLOG_EXPORTS
#define CSSIMLOG_API __declspec(dllexport)
#else
#define CSSIMLOG_API __declspec(dllimport)
#endif

#endif /* CSSIMLOG_EXPORT_H */
