#include "WaitSetDataSplDcps.h"
#include "ccpp_WaitSetData.h"

#include <v_copyIn.h>
#include <v_topic.h>
#include <os_stdlib.h>
#include <string.h>
#include <os_report.h>

v_copyin_result
__WaitSetData_Msg__copyIn(
c_base base,
const struct ::WaitSetData::Msg *from,
struct _WaitSetData_Msg *to)
{
	v_copyin_result result = V_COPYIN_RESULT_OK;
	(void)base;

	to->subjectId = (c_long)from->subjectId;
#ifdef OSPL_BOUNDS_CHECK
	if(from->systemId){
		to->systemId = c_stringNew_s(base, from->systemId);
		if(to->systemId == NULL) {
			result = V_COPYIN_RESULT_OUT_OF_MEMORY;
		}
	} else {
		OS_REPORT (OS_ERROR, "copyIn", 0,"Member 'WaitSetData::Msg.systemId' of type 'c_string' is NULL.");
		result = V_COPYIN_RESULT_INVALID;
	}
#else
	to->systemId = c_stringNew_s(base, from->systemId);
	if (to->systemId == NULL) {
		result = V_COPYIN_RESULT_OUT_OF_MEMORY;
	}
#endif
	to->time = (c_double)from->time;
#ifdef OSPL_BOUNDS_CHECK
	if(from->from){
		to->from = c_stringNew_s(base, from->from);
		if(to->from == NULL) {
			result = V_COPYIN_RESULT_OUT_OF_MEMORY;
		}
	} else {
		OS_REPORT (OS_ERROR, "copyIn", 0,"Member 'WaitSetData::Msg.from' of type 'c_string' is NULL.");
		result = V_COPYIN_RESULT_INVALID;
	}
#else
	to->from = c_stringNew_s(base, from->from);
	if (to->from == NULL) {
		result = V_COPYIN_RESULT_OUT_OF_MEMORY;
	}
#endif
#ifdef OSPL_BOUNDS_CHECK
	if(from->topicName){
		to->topicName = c_stringNew_s(base, from->topicName);
		if(to->topicName == NULL) {
			result = V_COPYIN_RESULT_OUT_OF_MEMORY;
		}
	} else {
		OS_REPORT (OS_ERROR, "copyIn", 0,"Member 'WaitSetData::Msg.topicName' of type 'c_string' is NULL.");
		result = V_COPYIN_RESULT_INVALID;
	}
#else
	to->topicName = c_stringNew_s(base, from->topicName);
	if (to->topicName == NULL) {
		result = V_COPYIN_RESULT_OUT_OF_MEMORY;
	}
#endif
#ifdef OSPL_BOUNDS_CHECK
	if(from->content){
		to->content = c_stringNew_s(base, from->content);
		if(to->content == NULL) {
			result = V_COPYIN_RESULT_OUT_OF_MEMORY;
		}
	} else {
		OS_REPORT (OS_ERROR, "copyIn", 0,"Member 'WaitSetData::Msg.content' of type 'c_string' is NULL.");
		result = V_COPYIN_RESULT_INVALID;
	}
#else
	to->content = c_stringNew_s(base, from->content);
	if (to->content == NULL) {
		result = V_COPYIN_RESULT_OUT_OF_MEMORY;
	}
#endif
	return result;
}

void
__WaitSetData_Msg__copyOut(
const void *_from,
void *_to)
{
	const struct _WaitSetData_Msg *from = (const struct _WaitSetData_Msg *)_from;
	struct ::WaitSetData::Msg *to = (struct ::WaitSetData::Msg *)_to;
	to->subjectId = (::DDS::Long)from->subjectId;
	to->systemId = DDS::string_dup(from->systemId ? from->systemId : "");
	to->time = (::DDS::Double)from->time;
	to->from = DDS::string_dup(from->from ? from->from : "");
	to->topicName = DDS::string_dup(from->topicName ? from->topicName : "");
	to->content = DDS::string_dup(from->content ? from->content : "");
}

