
#ifndef CS_SCENARIO_XML_H
#define CS_SCENARIO_XML_H

#include <map>
#include <string>
#include <unordered_map>
#include <fstream>

//#include "csscenario_xml_export.h"
#include "xml_define.h"
//#include "cssimlog/cssimlog.h"
//#include "csentity/csentity.h"

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include "rapidxml/rapidxml_utils.hpp"
using namespace rapidxml;

//class CSEntity;

class SimControlCSScenarioXML {

public:
	SimControlCSScenarioXML() = default;
	SimControlCSScenarioXML(const std::string& xml_path);
	~SimControlCSScenarioXML() = default;

	SimControlCSScenarioXML(const SimControlCSScenarioXML& other) = delete;
	SimControlCSScenarioXML& operator=(const SimControlCSScenarioXML& other) = delete;

	/*static CSScenarioXML& getInstance() {
		static CSScenarioXML instance;
		return instance;
	}*/

	bool ReadXML(const std::string& scenario_full_name);

	TopicDefineInfo GetTopicDefineInfo(const std::string &topic_name)const;
	TypeDefineInfo GetTypeDefineInfo(const std::string &type_name)const;
	PubSubItem GetPubSub(const std::string &type_name)const;
	std::string GetSystemId();
	std::string GetNodeName();
	std::vector<std::string> GetAllNode();

protected:
	//读取xml
	void ParserScenarioInfo(xml_node<char>*& parent_node);
	void ParserTypeDefine(xml_node<char>*& parent_ndoe);
	void ParserTopicDefine(xml_node<char>*& parent_ndoe);
	void ParserTopics(xml_node<char>*& parent_ndoe);
	void ParserPublishSubscribe(xml_node<char>*& parent_ndoe);
private:
	std::string file_path_;

	//基本信息
	std::string scenario_name_;
	std::string id_;
	std::string node_;
	std::string version_;

	//主题
	//主题名称-参数名称-参数类型
	//std::map < std::string, std::map < std::string, std::string >> topics_;
	std::unordered_map<std::string, TopicInfo> topics_;
	std::unordered_map<std::string, TypeDefineInfo> type_define_s_;
	std::unordered_map<std::string, TopicDefineInfo> topic_define_s_;

	std::map<std::string, PubSubItem> pub_sub_;

	std::vector<std::string> all_node_;
};

#endif