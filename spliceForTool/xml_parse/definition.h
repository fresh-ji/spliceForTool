
#ifndef _MYDEFINITION_
#define _MYDEFINITION_

#include <unordered_map>

using namespace std;

/**
* @brief	交互主题
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
* @brief	目前的统一引擎名
*/
#define ENGINENAME "SIMUengine777"

/**
* @brief	是否显示测试输出
*/
#define STDOUTTRACE

/**
* @param	开始时间
* @param	步长
*/
typedef void(*initTool)(double, double);

/**
* @param	当前时间
* @param	key
* @param	value
*/
typedef void(*setToTool)(double, char*, void*);

/**
* @param	当前时间
*/
typedef void(*setFinish)(double);

/**
*/
typedef void(*endTool)();

/**
* @brief	XML解析中的数据结构
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
