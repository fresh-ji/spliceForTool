#ifndef WAITSETDATASPLDCPS_H
#define WAITSETDATASPLDCPS_H

#include <c_base.h>
#include <c_misc.h>
#include <c_sync.h>
#include <c_collection.h>
#include <c_field.h>
#include <v_copyIn.h>

#include "WaitSetData.h"
#include "examples_export.h"

extern c_metaObject __WaitSetData_WaitSetData__load (c_base base);

extern const char *WaitSetData_Msg_metaDescriptor[];
extern const c_ulong WaitSetData_Msg_metaDescriptorArrLength;
extern const c_ulong WaitSetData_Msg_metaDescriptorLength;
extern c_metaObject __WaitSetData_Msg__load (c_base base);
struct _WaitSetData_Msg ;
extern OS_EXAMPLE_API v_copyin_result __WaitSetData_Msg__copyIn(c_type dbType, const class WaitSetData::Msg *from, struct _WaitSetData_Msg *to);
extern OS_EXAMPLE_API void __WaitSetData_Msg__copyOut(const void *_from, void *_to);
struct _WaitSetData_Msg {
    c_long subjectId;
    c_string systemId;
    c_double time;
    c_string from;
    c_string topicName;
    c_string content;
};

#undef OS_API
#endif
