
#ifndef CS_SCENARIO_XML_H
#define CS_SCENARIO_XML_H

#include <map>

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include "rapidxml/rapidxml_utils.hpp"

#include "definition.h"

using namespace rapidxml;

class CSEntity;

class CSScenarioXML {

public:
	CSScenarioXML() = default;
	CSScenarioXML(const string& xml_path);
	~CSScenarioXML() = default;

	CSScenarioXML(const CSScenarioXML& other) = delete;
	CSScenarioXML& operator=(const CSScenarioXML& other) = delete;

public:
	bool ReadXML(const string& scenario_full_name);

	string GetSystemId();
	string GetNodeName();
	vector<string> GetAllNode();
	TypeDefineInfo GetTypeDefineInfo(const string &type_name) const;
	TopicDefineInfo GetTopicDefineInfo(const string &topic_name) const;
	PubSubItem GetPubSub(const string &type_name) const;

protected:

	bool ParserScenarioInfo(xml_node<char>*& parent_node);
	bool ParserTypeDefine(xml_node<char>*& parent_ndoe);
	bool ParserTopicDefine(xml_node<char>*& parent_ndoe);
	bool ParserPublishSubscribe(xml_node<char>*& parent_ndoe);

private:
	string file_path_;

	// 基本信息
	string scenario_name_;
	string id_;
	string node_;
	string version_;
	vector<string> all_node_;

	// 主题
	// 主题名称-参数名称-参数类型
	unordered_map<string, TopicInfo> topics_;
	unordered_map<string, TypeDefineInfo> type_define_s_;
	unordered_map<string, TopicDefineInfo> topic_define_s_;

	// 发布订阅
	map<string, PubSubItem> pub_sub_;
};

#endif
