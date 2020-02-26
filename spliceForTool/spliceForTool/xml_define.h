
#ifndef XML_DEFINE_H_
#define XML_DEFINE_H_

#include <map>
#include <set>
#include <string>
#include <unordered_map>

//用户自定义结构体参数
typedef struct _UDStructParamItem {
	//参数名称
	std::string param_name;
	//参数类型
	std::string param_type;
	//显示名称
	std::string display_name;
	//用途
	std::string usage;
	//值
	std::string value;

} UDStructParamItem;

//模型参数
typedef struct _ModelParamItem {
	//参数名称
	std::string param_name;
	//参数类型
	std::string param_type;
	//显示名称
	std::string display_name;
	//用途
	std::string usage;
	//值
	std::string value;

} ModelParamItem;

//子模型
typedef struct _SubModelItem {
	std::string uuid;
	std::string model_name;
	std::string model_type;
	std::string display_name;
	std::string model_classification;
	//模型参数
	std::map<std::string, ModelParamItem> params;
	//模型参数显示顺序
	std::vector<std::string> param_orders;
} SubModelItem;


//模型
typedef struct _ModelItem {
	//模型基本属性及参数
	SubModelItem base_info;
	//子模型
	std::map<std::string, SubModelItem> sub_models;
} ModelItem;

//订阅-发布数据结构
typedef struct _TopicItem {
	std::string topic_name; //主题名称
	std::string topic_param_name;//主题参数名称
	std::string model_param_name;//模型参数名称
} TopicItem;


//typedef struct _PubSubItem {
//  //模型参数名称-主题名称-主题参数
//  std::map < std::string, std::map<std::string, std::string>> publish;
//  std::map < std::string, std::pair <std::string, std::string>> subscribe;
//} PubSubItem;

//节点信息
typedef struct _NetNodeInfo {
	std::string describe; //描述信息
	std::string type;			//节点类型
	std::set<std::string> entities;//实体句柄
} NetNodeInfo;

//主题信息
typedef struct _TopicInfo {
	std::string topic_name;//主题名称
	std::string topic_type;//主题类型
	//参数名称-参数类型
	std::unordered_map<std::string, std::string> params;
} TopicInfo;

//主题定义信息
typedef struct _TopicDefineInfo {
	std::string topic_name;//主题名称
	//参数名称-参数类型
	std::unordered_map<std::string, std::string> params;
} TopicDefineInfo;

//类型定义信息
typedef struct _TypeDefineInfo {
	std::string type_name;//主题名称
	//参数名称-参数类型
	std::unordered_map<std::string, std::string> params;
} TypeDefineInfo;

typedef struct _PubSubItem {
	//模型参数名称-主题名称-主题参数
	std::vector < std::string> publish;
	std::vector < std::string> subscribe;
} PubSubItem;

#endif
