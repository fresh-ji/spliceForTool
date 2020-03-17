#ifndef DDS_PREDEFTYPESDCPS_H
#define DDS_PREDEFTYPESDCPS_H

#include <dds_primitive_types.h>
#include <stdint.h>
#include <stdbool.h>

#if defined (__cplusplus)
extern "C" {
#endif
#include "dds_c99_if.h"
#define DDS_sequence_octet__alloc DDS_sequence_octet__alloc
#define DDS_sequence_octet_allocbuf DDS_sequence_octet_allocbuf

typedef DDS_sequence_octet DDS_ByteSeq;
C99_BUILTIN DDS_ByteSeq *DDS_ByteSeq__alloc (void);
C99_BUILTIN uint8_t *DDS_ByteSeq_allocbuf (uint32_t len);

#ifndef _DDS_String_defined
#define _DDS_String_defined
#ifdef __cplusplus
struct DDS_String;
#else /* __cplusplus */
typedef struct DDS_String DDS_String;
#endif /* __cplusplus */
#endif /* _DDS_String_defined */
C99_BUILTIN DDS_String *DDS_String__alloc (void);

struct DDS_String {
    char * value;
};

#ifndef _DDS_KeyedString_defined
#define _DDS_KeyedString_defined
#ifdef __cplusplus
struct DDS_KeyedString;
#else /* __cplusplus */
typedef struct DDS_KeyedString DDS_KeyedString;
#endif /* __cplusplus */
#endif /* _DDS_KeyedString_defined */
C99_BUILTIN DDS_KeyedString *DDS_KeyedString__alloc (void);

struct DDS_KeyedString {
    char * key;
    char * value;
};

#ifndef _DDS_Bytes_defined
#define _DDS_Bytes_defined
#ifdef __cplusplus
struct DDS_Bytes;
#else /* __cplusplus */
typedef struct DDS_Bytes DDS_Bytes;
#endif /* __cplusplus */
#endif /* _DDS_Bytes_defined */
C99_BUILTIN DDS_Bytes *DDS_Bytes__alloc (void);

struct DDS_Bytes {
    DDS_ByteSeq value;
};

#ifndef _DDS_KeyedBytes_defined
#define _DDS_KeyedBytes_defined
#ifdef __cplusplus
struct DDS_KeyedBytes;
#else /* __cplusplus */
typedef struct DDS_KeyedBytes DDS_KeyedBytes;
#endif /* __cplusplus */
#endif /* _DDS_KeyedBytes_defined */
C99_BUILTIN DDS_KeyedBytes *DDS_KeyedBytes__alloc (void);

struct DDS_KeyedBytes {
    char * key;
    DDS_ByteSeq value;
};

#if defined (__cplusplus)
}
#endif

#endif
