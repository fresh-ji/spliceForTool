
#include "stdafx.h"
#include "XMLapi.h"

#include "cssimlog.h"
#include "cssimlog_export.h"

CSScenarioXML::CSScenarioXML(
	const string& xml_path) : file_path_(xml_path) {
	//already_read_publish_subscribe_xml_ = false;
}

bool CSScenarioXML::ReadXML(const string& scenario_full_name) {

	string msg;
	try {
		msg = "start parsing xml : " + scenario_full_name;

		rapidxml::file<> content(scenario_full_name.c_str());

		xml_document<> doc;
		doc.parse<0>(content.data());

		xml_node<char>* millitary_node =
			doc.first_node("MilitaryScenario");
		if (!millitary_node) {
			LogDDSErr("no MilitaryScenario");
			return false;
		}
		// 基本信息
		if (!ParserScenarioInfo(millitary_node)) {
			LogDDSErr("ParserScenarioInfo wrong");
			return false;
		}
		//自定义类型
		if (!ParserTypeDefine(millitary_node)) {
			LogDDSErr("ParserTypeDefine wrong");
			return false;
		}
		//主题
		if (!ParserTopicDefine(millitary_node)) {
			LogDDSErr("ParserTopicDefine wrong");
			return false;
		}
		//发布订阅关系
		if (!ParserPublishSubscribe(millitary_node)) {
			LogDDSErr("ParserPublishSubscribe wrong");
			return false;
		}
		return true;
	}
	catch (const parse_error& e) {
		msg = "parsing xml error,";
		msg += e.what();
		LogDDSErr(msg);
		return false;
	}
	catch (const exception& e) {
		msg = "paring xml exception,";
		msg += e.what();
		LogDDSErr(msg);
		return false;
	}
	catch (...) {
		LogDDSErr("paring xml unknown wrong");
		return false;
	}
	return true;
}

bool CSScenarioXML::ParserScenarioInfo(xml_node<char>*& parent_node) {

	xml_node<> *info_node = parent_node->first_node("ScenarioInfo");
	if (!info_node) {
		LogDDSErr("no ScenarioInfo");
		return false;
	}
	xml_node<> *name_node = info_node->first_node("Name");
	if (name_node) {
		auto value = name_node->value();
		if (value) {
			scenario_name_ = value;
		}
		else {
			LogDDSErr("Name no value");
			return false;
		}
	}
	else {
		LogDDSErr("no Name");
		return false;
	}
	xml_node<> *id_node = info_node->first_node("Id");
	if (id_node) {
		auto value = id_node->value();
		if (value) {
			id_ = value;
		}
		else {
			LogDDSErr("Id no value");
			return false;
		}
	}
	else {
		LogDDSErr("no Id");
		return false;
	}
	xml_node<> *node_node = info_node->first_node("Node");
	if (node_node) {
		auto value = node_node->value();
		if (value) {
			node_ = value;
		}
		else {
			LogDDSErr("Node no value");
			return false;
		}
	}
	else {
		LogDDSErr("no Node");
		return false;
	}
	return true;
}

bool CSScenarioXML::ParserTypeDefine(xml_node<char>*& parent_ndoe) {

	xml_node<> *type_define_node = parent_ndoe->first_node("TypeDefine");
	if (!type_define_node) {
		LogDDSErr("no TypeDefine");
		return false;
	}
	xml_attribute<> *att = nullptr;

	for (auto it = type_define_node->first_node("Type");
		it; it = it->next_sibling()) {

		TypeDefineInfo type_define_info;

		//获取自定义类型名称
		att = it->first_attribute("name");
		if (att) {
			type_define_info.type_name = att->value();
		}
		else {
			LogDDSErr("Type-name no attribute");
			return false;
		}
		//获取自定义类型参数
		string param_name, param_type;
		for (auto param = it->first_node("Parameter");
			param; param = param->next_sibling()) {

			//参数名称
			att = param->first_attribute("name");
			if (att) {
				param_name = att->value();
			}
			else {
				LogDDSErr("Type-Parameter-name no attribute");
				return false;
			}
			//参数类型
			att = param->first_attribute("type");
			if (att) {
				param_type = att->value();
			}
			else {
				LogDDSErr("Type-Parameter-type no attribute");
				return false;
			}
			type_define_info.params.emplace(param_name, param_type);
		}
		type_define_s_[type_define_info.type_name] = type_define_info;
	}
	return true;
}

bool CSScenarioXML::ParserTopicDefine(xml_node<char>*& parent_ndoe) {

	xml_node<> *topics_node = parent_ndoe->first_node("Topics");
	if (!topics_node) {
		LogDDSErr("no Topics");
		return false;
	}

	xml_attribute<> *att = nullptr;

	for (auto it = topics_node->first_node("Topic");
		it; it = it->next_sibling()) {

		TopicDefineInfo topic_define_info;
		//获取主题名称
		att = it->first_attribute("name");
		if (att) {
			topic_define_info.topic_name = att->value();
		}
		else {
			LogDDSErr("Topic-name no attribute");
			return false;
		}
		//获取主题参数类型(主题名就是参数名，主题类型就是参数类型)
		att = it->first_attribute("type");
		if (att) {
			topic_define_info.params[topic_define_info.topic_name] = att->value();
		}
		else {
			// type 有可能没有
			continue;
		}
		topic_define_s_[topic_define_info.topic_name] = topic_define_info;
	}
	return true;
}

bool CSScenarioXML::ParserPublishSubscribe(xml_node<char>*& parent_node) {

	xml_node<> *models_node = parent_node->first_node("Models");

	if (!models_node) {
		LogDDSErr("no Models");
		return false;
	}

	string model_name;

	for (auto it = models_node->first_node("Model");
		it; it = it->next_sibling()) {

		xml_attribute<> *att = it->first_attribute("name");
		if (!att) {
			LogDDSErr("Model-name no attribute");
			return false;
		}

		model_name = att->value();

		PubSubItem pub_sub_item;
		string topic_name;

		xml_node<> *pub_node = it->first_node("Publish");
		if (pub_node) {
			//发布关系
			for (auto pub_topic = pub_node->first_node("Topic");
				pub_topic; pub_topic = pub_topic->next_sibling()) {

				att = pub_topic->first_attribute("topicName");
				if (!att) {
					LogDDSErr("Model-Publish-Topic-topicName no attribute");
					return false;
				}
				topic_name = att->value();
				pub_sub_item.publish.push_back(topic_name);
			}
		}
		else {
			LogDDSErr("no Publish");
			return false;
		}

		xml_node<> *sub_node = it->first_node("Subscribe");
		if (sub_node) {
			//订阅关系
			for (auto sub_topic = sub_node->first_node("Topic");
				sub_topic; sub_topic = sub_topic->next_sibling()) {

				att = sub_topic->first_attribute("topicName");
				if (!att) {
					LogDDSErr("Model-Subscribe-Topic-topicName no attribute");
					return false;
				}
				topic_name = att->value();
				pub_sub_item.subscribe.push_back(topic_name);
			}
		}
		else {
			LogDDSErr("no Subscribe");
			return false;
		}
		pub_sub_[model_name] = pub_sub_item;
		if (strcmp(ENGINENAME, model_name.c_str())) {
			all_node_.push_back(model_name);
		}
	}
	return true;
}

TopicDefineInfo CSScenarioXML::GetTopicDefineInfo(const string &topic_name) const {
	auto it = topic_define_s_.find(topic_name);
	if (it != topic_define_s_.end()) {
		return it->second;
	}
	else {
		TopicDefineInfo tmp;
		LogDDSErr("TopicDefineInfo get none");
		return tmp;
	}
}

TypeDefineInfo CSScenarioXML::GetTypeDefineInfo(const string &type_name) const {
	auto it = type_define_s_.find(type_name);
	if (it != type_define_s_.end()) {
		return it->second;
	}
	else {
		TypeDefineInfo tmp;
		LogDDSErr("TypeDefineInfo get none");
		return tmp;
	}
}

PubSubItem CSScenarioXML::GetPubSub(const string &node_name) const {
	auto it = pub_sub_.find(node_name);
	if (it != pub_sub_.end()) {
		return it->second;
	}
	else {
		PubSubItem tmp;
		LogDDSErr("PubSubItem get none");
		return tmp;
	}
}

string CSScenarioXML::GetSystemId() {
	return id_;
}

string CSScenarioXML::GetNodeName() {
	return node_;
}

vector<string> CSScenarioXML::GetAllNode() {
	return all_node_;
}
