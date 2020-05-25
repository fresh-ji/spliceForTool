
#ifndef _MYDEFINITION_
#define _MYDEFINITION_

#include <unordered_map>

using namespace std;

/**
* @brief	��������
*/
#define ACQUIRE_READY_STATE "acquire_ready_state"
#define NODE_READY "node_ready"
#define INITIAL_FEDERATE "initial_federate"
#define ADVANCE_REQUEST "advance_request"
#define ADVANCE_GRANT "advance_grant"
// #define SIMULATION_RUN "simulation_run"
#define SIMULATION_END "simulation_end"
#define DUPLICATED_ONLINE "duplicated_online"
#define NODE_END "node_end"

/**
* @brief	Ŀǰ��ͳһ������
*/
#define ENGINENAME "SIMUengine777"

/**
* @brief	�Ƿ���ʾ�������
*/
#define STDOUTTRACE

/**
* @param	��ʼʱ��
* @param	����
*/
typedef void(*initTool)(double, double);

/**
* @param	��ǰʱ��
* @param	key
* @param	value
*/
typedef void(*setToTool)(double, char*, void*);

/**
* @param	��ǰʱ��
*/
typedef void(*setFinish)(double);

/**
*/
typedef void(*endTool)();

/**
* @brief	XML�����е����ݽṹ
*/
typedef struct _TopicInfo {
	string topic_type;
	unordered_map<string, string> params;
} TopicInfo;

typedef struct _TopicDefineInfo {
	string topic_name;
	unordered_map<string, string> params;
} TopicDefineInfo;

typedef struct _TypeDefineInfo {
	string type_name;
	unordered_map<string, string> params;
} TypeDefineInfo;

typedef struct _PubSubItem {
	vector<string> publish;
	vector<string> subscribe;
} PubSubItem;

#endif
