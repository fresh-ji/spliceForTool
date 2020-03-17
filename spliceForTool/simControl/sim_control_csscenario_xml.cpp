
//#include "stdafx.h"
#include "sim_control_csscenario_xml.h"

SimControlCSScenarioXML::SimControlCSScenarioXML(const std::string& xml_path) :file_path_(xml_path) {
	//already_read_publish_subscribe_xml_ = false;
}

bool SimControlCSScenarioXML::ReadXML(const std::string& scenario_full_name) {

	bool rv = false;

	try {
		std::string msg = "start parse xml:" + scenario_full_name;
		LogSEInfo(msg);

		rapidxml::file<> content(scenario_full_name.c_str());

		xml_document<> doc;
		doc.parse<0>(content.data());

		xml_node<char>* millitary_node = doc.first_node("MilitaryScenario");
		if (!millitary_node) {
			return false;
		}

		ParserScenarioInfo(millitary_node);

		//自定义类型
		ParserTypeDefine(millitary_node);

		//主题
		ParserTopicDefine(millitary_node);

		//发布订阅关系
		ParserPublishSubscribe(millitary_node);

	}
	catch (const parse_error& e) {
		std::string error_info = "解析想定XML出错，";
		error_info += e.what();
		LogSEErr(error_info);
		return false;
	}
	catch (const std::exception& e) {
		std::string error_info = "解析想定XML出错，";
		error_info += e.what();
		LogSEErr(error_info);
		return false;
	}
	catch (...) {
		LogSEErr("解析想定XML出错");
		return false;
	}

	std::string msg = "end parse xml:" + scenario_full_name + "successed!";
	LogSEInfo(msg);
	return true;
}

void SimControlCSScenarioXML::ParserScenarioInfo(xml_node<char>*& parent_node) {
	xml_node<> *info_node = parent_node->first_node("ScenarioInfo");
	if (!info_node) {
		return;
	}

	xml_node<> *name_node = info_node->first_node("Name");
	if (name_node) {
		auto value = name_node->value();
		if (value) {
			scenario_name_ = value;
		}
	}

	xml_node<> *id_node = info_node->first_node("Id");
	if (id_node) {
		auto value = id_node->value();
		if (value) {
			id_ = value;
		}
	}

	xml_node<> *node_node = info_node->first_node("Node");
	if (node_node) {
		auto value = node_node->value();
		if (value) {
			node_ = value;
		}
	}

	std::string msg = "parse ScenarioInfo successed";
	LogSEInfo(msg);
}

void SimControlCSScenarioXML::ParserTypeDefine(xml_node<char>*& parent_ndoe) {

	xml_node<> *type_define_node = parent_ndoe->first_node("TypeDefine");
	if (!type_define_node) {
		return;
	}

	xml_attribute<> *att = nullptr;

	for (auto it = type_define_node->first_node("Type"); it;
		it = it->next_sibling()) {
		TypeDefineInfo type_define_info;
		//获取自定义类型名称
		att = it->first_attribute("name");
		if (att) {
			type_define_info.type_name = att->value();
		}
		else {
			LogSEErr("自定义类型名称为空");
			continue;
		}

		//获取自定义类型参数
		std::string param_name, param_type;
		for (auto param = it->first_node("Parameter"); param;
			param = param->next_sibling()) {
			//参数名称
			att = param->first_attribute("name");
			if (att) {
				param_name = att->value();
			}
			else {
				LogSEErr("自定义类型-参数名称为空");
				continue;
			}
			//参数类型
			att = param->first_attribute("type");
			if (att) {
				param_type = att->value();
			}
			else {
				LogSEErr("自定义类型-参数类型为空");
				continue;
			}

			type_define_info.params.emplace(param_name, param_type);
		}
		type_define_s_[type_define_info.type_name] = type_define_info;
	}

	std::string msg = "parse TypeDefine successed";
	LogSEInfo(msg);
}

void SimControlCSScenarioXML::ParserTopicDefine(xml_node<char>*& parent_ndoe) {

	xml_node<> *topics_node = parent_ndoe->first_node("Topics");
	if (!topics_node) {
		return;
	}

	xml_attribute<> *att = nullptr;

	for (auto it = topics_node->first_node("Topic"); it;
		it = it->next_sibling()) {
		TopicDefineInfo topic_define_info;
		//获取主题名称
		att = it->first_attribute("name");
		if (att) {
			topic_define_info.topic_name = att->value();
		}
		else {
			LogSEErr("主题名称为空");
			continue;
		}

		//获取主题参数类型(主题名就是参数名，主题类型就是参数类型)
		att = it->first_attribute("type");
		if (att) {
			topic_define_info.params[topic_define_info.topic_name] = att->value();
		}
		else {
			LogSEErr("主题类型为空");
			continue;
		}

		topic_define_s_[topic_define_info.topic_name] = topic_define_info;
	}

	std::string msg = "parse Topics successed";
	LogSEInfo(msg);
}

void SimControlCSScenarioXML::ParserPublishSubscribe(xml_node<char>*& parent_node) {

	xml_node<> *models_node = parent_node->first_node("Models");

	if (!models_node) {
		return;
	}

	std::string model_name;

	for (auto it = models_node->first_node("Model"); it;
		it = it->next_sibling()) {

		xml_attribute<> *att = it->first_attribute("name");
		if (!att) {
			continue;
		}

		model_name = att->value();

		PubSubItem pub_sub_item;
		std::string topic_name;

		xml_node<> *pub_node = it->first_node("Publish");
		if (pub_node) {
			//发布关系
			for (auto pub_topic = pub_node->first_node("Topic"); pub_topic;
				pub_topic = pub_topic->next_sibling()) {

				att = pub_topic->first_attribute("topicName");
				if (!att) {
					continue;
				}
				topic_name = att->value();
				pub_sub_item.publish.push_back(topic_name);
			}
		}

		xml_node<> *sub_node = it->first_node("Subscribe");
		if (sub_node) {
			//订阅关系
			for (auto sub_topic = sub_node->first_node("Topic"); sub_topic;
				sub_topic = sub_topic->next_sibling()) {
				att = sub_topic->first_attribute("topicName");
				if (!att) {
					continue;
				}
				topic_name = att->value();

				pub_sub_item.subscribe.push_back(topic_name);
			}
		}

		pub_sub_[model_name] = pub_sub_item;
		all_node_.push_back(model_name);
		//CSScenario::Instance().AddModelPublishSubscribe(model_uuid, std::move(pub_sub));
	}

	std::string msg = "parse Models successed";
	LogSEInfo(msg);
}

TopicDefineInfo  SimControlCSScenarioXML::GetTopicDefineInfo(const std::string &topic_name)const {
	auto it = topic_define_s_.find(topic_name);
	if (it != topic_define_s_.end()) {
		return it->second;
	}
	else{
		TopicDefineInfo tmp;
		return tmp;
	}
}

TypeDefineInfo SimControlCSScenarioXML::GetTypeDefineInfo(const std::string &type_name)const {
	auto it = type_define_s_.find(type_name);
	if (it != type_define_s_.end()) {
		return it->second;
	}
	else {
		TypeDefineInfo tmp;
		return tmp;
	}
}

PubSubItem SimControlCSScenarioXML::GetPubSub(const std::string &node_name)const{
	auto it = pub_sub_.find(node_name);
	if (it != pub_sub_.end()) {
		return it->second;
	}
	else {
		PubSubItem tmp;
		return tmp;
	}
}

std::string SimControlCSScenarioXML::GetSystemId(){
	return id_;
}

std::string SimControlCSScenarioXML::GetNodeName(){
	return node_;
}

std::vector<std::string> SimControlCSScenarioXML::GetAllNode() {
	return all_node_;
}