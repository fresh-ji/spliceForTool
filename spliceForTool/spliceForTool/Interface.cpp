
#include "stdafx.h"
#include "Interface.h"
#include <stdlib.h>
#include <malloc.h>

class ReadCondHandler {
public:
	/**
	* @param dataState The dataState on which to filter the samples
	*/
	ReadCondHandler(dds::sub::status::DataState& dataState)
		: dataState(dataState) {}
	void operator() (const dds::sub::cond::ReadCondition& cond) {
		/** retrieve the DataState from the condition */
		dds::sub::status::DataState dataState = cond.state_filter();
		/** retrieve the associated reader from the condition */
		dds::sub::DataReader<Msg> dr = cond.data_reader();

		dds::sub::LoanedSamples<Msg> samples = dr.select().state(dataState).take();
		// dds::sub::LoanedSamples<Msg> samples = dr.select().content(cond).take();
		// dds::sub::LoanedSamples<Msg> samples = dr.take();

		for (dds::sub::LoanedSamples<Msg>::const_iterator sample = samples.begin();
			sample < samples.end(); ++sample) {
			if ((*sample).info().valid()) {
				Interface::getInstance().process(sample->data());
			}
		}
	}
private:
	dds::sub::status::DataState& dataState;
};

bool wsServe(dds::core::cond::WaitSet waitSet, string systemId) {
	cout << "[checked] <" << systemId << "> "
		<< "dds detached thread starts well" << endl;
	while (1) {
		try {
			waitSet.dispatch();
		}
		catch (const dds::core::TimeoutError e) {
			cout << "[error] <" << systemId << "> "
				"dds thread:" << e.what() << endl;
			return false;
		}
	}
	cout << "[checked] <" << systemId << "> "
		<< "dds detached thread ends" << endl;
	return true;
}

Interface::Interface() {
	systemRunId = org::opensplice::domain::default_id();
}

bool Interface::start(string configName,
	initTool p_initTool, setToTool p_setToTool,
	setFinish p_setFinish, endTool p_endTool) {

	try {
		// 1.Read Config
		if (!xml_parser_.ReadXML(configName)) {
			cout << "[error] system config fail" << endl;
			return false;
		}

		systemId = xml_parser_.GetSystemId();
		nodeName = xml_parser_.GetNodeName();

		PubSubItem pub_sub = xml_parser_.GetPubSub(nodeName);
		pubNames = pub_sub.publish;
		subNames = pub_sub.subscribe;

		// 2.Parameters
		currentTime = 0.0;
		this->p_initTool = p_initTool;
		this->p_setToTool = p_setToTool;
		this->p_setFinish = p_setFinish;
		this->p_endTool = p_endTool;
		cout << "[checked] <" << systemId << "> "
			<< "got callback parameters" << endl;

		// 3.DDS
		if (!startServerDDS()) {
			cout << "[error] <" << systemId << "> "
				<< "start dds fail" << endl;
			return false;
		}

		thread th(wsServe, waitSet, systemId);
		th.detach();

		cout << "-----CONGRATULATIONS, ALMOST DONE!-----" << endl;

		// TODO 处理
		Sleep(1000);
		return publish(NODE_READY, "me");
	}
	catch (runtime_error& e) {
		cout << "[error] <" << systemId << "> "
			<< "runtime:" << e.what() << endl;
		return false;
	}
	catch (exception &e) {
		cout << "[exception] <" << systemId << "> "
			<< e.what() << endl;
		return false;
	}
}

bool Interface::setValue(string topic_name, void* data_ptr) {
	string data = ConvertTypeData2Json(topic_name, data_ptr);
	if (!publish(topic_name.c_str(), data)) {
		cout << "[error] <" << systemId << "> "
			<< "data sent fail at "
			<< to_string(currentTime) << endl;
		return false;
	}
	return true;
}

bool Interface::advance() {
	if (!publish(ADVANCE_REQUEST, to_string(currentTime))) {
		cout << "[error] <" << systemId << "> "
			<< "advance sent fail at "
			<< to_string(currentTime) << endl;
		return false;
	}
	cout << "<" << systemId << "> advance send successed at "
		<< to_string(currentTime) << endl;

	return true;
}

bool Interface::end() {
	// TODO 删资源，目前都是引擎结束这一切
	return true;

}

bool Interface::process(Msg messageIn) {

	string str;
	string str_time = to_string(messageIn.time());
	string tName = messageIn.topicName();

	str = "RECEIVE <";
	str = str + tName + "> FROM <" + messageIn.from()
		+ "> AT <" + str_time + ">";
	cout << str << endl;

	string sName = messageIn.systemId();
	if (sName != systemId) {
		cout << "[error] <" << systemId << "> "
			<< "the message is not for me" << endl;
		return false;
	}

	// prevent history data
	if ((currentTime - messageIn.time()) > 10e-5) {
		str = "Old Data {";
		str = str + str_time + "} at {"
			+ to_string(currentTime) + "}";
		cout << "[error] <" << systemId << "> "
			<< str << endl;
		return false;
	}

	if (tName == ACQUIRE_READY_STATE) {
		publish(NODE_READY, "me");
	}
	else if (tName == INITIAL_FEDERATE) {
		currentTime = messageIn.time();

		stringstream ss;
		ss << messageIn.content();
		ss >> step;

		(*p_initTool)(currentTime, step);
	}
	else if (tName == ADVANCE_GRANT) {
		map<string, string> tempMap = dataMap;
		dataMap = backupDataMap;
		backupDataMap.clear();

		map<string, string>::iterator iter;
		iter = tempMap.begin();
		while (iter != tempMap.end()) {
			string key = iter->first;
			char *cstr = new char[key.length() + 1];
			strcpy(cstr, key.c_str());
			string value = iter->second;
			char *cstr2 = new char[value.length() + 1];
			strcpy(cstr2, value.c_str());

			char* datac = ConvertJson2TypeData(key, value);
			//char *datac = new char[data.length() + 1];
			//strcpy(datac, data.c_str());

			(*p_setToTool)(currentTime, cstr, (void*)datac);
			delete[] cstr;
			delete[] cstr2;
			iter++;
		}
		currentTime = messageIn.time();
		(*p_setFinish)(currentTime);
	}
	else if (tName == SIMULATION_END) {
		// TODO 删资源，目前都是引擎结束这一切
		(*p_endTool)();
	}
	else if (tName == SIMULATION_RUN) {
		//advance();
	}
	else {
		// actual data
		if (abs(messageIn.time() - currentTime) < 10e-5) {
			dataMap.insert(make_pair(messageIn.topicName(), messageIn.content()));
		}
		else {
			backupDataMap.insert(make_pair(messageIn.topicName(), messageIn.content()));
		}
	}
	return true;
}



bool Interface::startServerDDS() {

	dds::domain::DomainParticipant dp(systemRunId);

	dds::pub::qos::PublisherQos pubQos
		= dp.default_publisher_qos()
		<< dds::core::policy::Partition("WaitSet example");
	dds::pub::Publisher pub(dp, pubQos);

	dds::sub::qos::SubscriberQos subQos
		= dp.default_subscriber_qos()
		<< dds::core::policy::Partition("WaitSet example");
	dds::sub::Subscriber sub(dp, subQos);

	dds::topic::qos::TopicQos topicQos = dp.default_topic_qos();

	for (auto n : pubNames) {
		dds::topic::Topic<Msg> topic(dp, (const string &)n, topicQos);

		dds::pub::qos::DataWriterQos dwqos = topic.qos();
		dwqos << dds::core::policy::WriterDataLifecycle
			::AutoDisposeUnregisteredInstances();
		dds::pub::DataWriter<Msg> dw(pub, topic, dwqos);
		writers.insert(make_pair(n, dw));
	}

	for (auto n : subNames) {
		dds::topic::Topic<Msg> topic(dp, (const string &)n, topicQos);

		dds::sub::qos::DataReaderQos drqos = topic.qos();
		dds::sub::DataReader<Msg> dr(sub, topic, drqos);
		readers.insert(make_pair(n, dr));

		dds::sub::status::DataState *newDataState
			= new dds::sub::status::DataState();
		(*newDataState) << dds::sub::status::SampleState::not_read()
			<< dds::sub::status::ViewState::new_view()
			<< dds::sub::status::InstanceState::any();
		ReadCondHandler *readCondHandler =
			new ReadCondHandler(*newDataState);
		dds::sub::cond::ReadCondition readCond(
			dr, *newDataState, *readCondHandler);

		waitSet += readCond;
	}
	return true;
}

bool Interface::publish(string topic, string data) {
	random_device rd;
	mt19937 mt(rd());

	Msg message;
	message.subjectId() = mt();
	message.systemId() = systemId;
	message.from() = nodeName;
	message.time() = currentTime;
	message.topicName() = topic;
	message.content() = data;

	dds::pub::DataWriter<Msg> writer = writers.at(topic);
	writer << message;
	return true;
}

string Interface::ConvertTypeData2Json(string topic_name, void* data_ptr) {

	TopicDefineInfo topic_define_info = xml_parser_.GetTopicDefineInfo(topic_name);
	std::unordered_map<std::string, std::string> params = topic_define_info.params;

	rapidjson::StringBuffer buf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
	writer.StartObject();

	if (data_ptr == NULL)
	{
		return  "null";
	}

	for (auto p : params) {
		std::string param_name = p.first;
		std::string param_type = p.second;
		if (param_type == "int32_t") {
			int data = *(int*)data_ptr;
			writer.Key(param_name.c_str()); writer.Int(data);
		}
		else if (param_type == "double") {
			double data = *(double*)data_ptr;
			writer.Key(param_name.c_str()); writer.Double(data);
		}
		else if (param_type == "string") {
			std::string data = (char*)data_ptr;
			writer.Key(param_name.c_str()); writer.String(data.c_str());
		}
		else {
			TypeDefineInfo type_define_info = xml_parser_.GetTypeDefineInfo(param_type);
			std::unordered_map<std::string, std::string> type_params = type_define_info.params;
			writer.Key(param_name.c_str());
			writer.StartObject();
			int index = 0;
			for (auto param : type_params) {
				std::string struct_param_name = param.first;
				std::string struct_param_type = param.second;
				if (struct_param_type == "int32_t") {
					int data = *((int*)((char*)data_ptr + index));
					index = index + sizeof(int32_t);
					writer.Key(struct_param_name.c_str()); writer.Int(data);
				}
				else if (struct_param_type == "double") {
					double data = *((double*)((char*)data_ptr + index));
					index = index + sizeof(double);
					writer.Key(struct_param_name.c_str()); writer.Double(data);
				}
				else if (struct_param_type == "string") {
					char buffer[1024];
					memset(buffer, 0, sizeof(buffer));
					int len = strlen((char*)((char*)data_ptr + index));
					memcpy(buffer, (char*)data_ptr + index, len);
					std::string data = buffer;
					index = index + len + 1;
					writer.Key(param_name.c_str()); writer.String(data.c_str());
				}
			}
			writer.EndObject();
		}
	}
	writer.EndObject();
	const char* json_content = buf.GetString();
	fprintf(stdout, "json content: %s\n", json_content);
	std::string str_data = json_content;
	return str_data;
}

char* Interface::ConvertJson2TypeData(string topic_name, string data) {

	TopicDefineInfo topic_define_info = xml_parser_.GetTopicDefineInfo(topic_name);
	std::unordered_map<std::string, std::string> params = topic_define_info.params;

	char *buffer = (char*)malloc(sizeof(char) * 1024);
	memset(buffer, 0, sizeof(buffer));
	int index = 0;

	if (params.size() <= 0) {
		return buffer;
	}

	std::string json_content = data;
	rapidjson::Document dom;
	if (!dom.Parse(json_content.c_str()).HasParseError()) {
		for (auto p : params) {
			std::string param_name = p.first;
			std::string param_type = p.second;
			if (param_type == "int32_t") {
				if (dom.HasMember(param_name.c_str()) && dom[param_name.c_str()].IsInt()) {
					int data = dom[param_name.c_str()].GetInt();
					memcpy(buffer + index, (char*)&data, sizeof(int32_t));
					index = index + sizeof(int32_t);
				}
			}
			else if (param_type == "double") {
				if (dom.HasMember(param_name.c_str()) && dom[param_name.c_str()].IsDouble()) {
					double data = dom[param_name.c_str()].GetDouble();
					memcpy(buffer + index, (char*)&data, sizeof(double));
					index = index + sizeof(double);
				}
			}
			else if (param_type == "string") {
				if (dom.HasMember(param_name.c_str()) && dom[param_name.c_str()].IsString()) {
					std::string data = dom[param_name.c_str()].GetString();
					int len = data.length();
					char *tmp = new char[len + 1];
					strcpy(tmp, data.c_str());

					memcpy(buffer + index, (char*)tmp, len + 1);
					index = index + len + 1;
				}
			}
			else {
				TypeDefineInfo type_define_info = xml_parser_.GetTypeDefineInfo(param_type);
				std::unordered_map<std::string, std::string> type_params = type_define_info.params;
				rapidjson::Value struct_obj;

				if (dom.HasMember(param_name.c_str()) && dom[param_name.c_str()].IsObject()) {
					struct_obj = dom[param_name.c_str()];
				}

				for (auto param : type_params) {
					std::string struct_param_name = param.first;
					std::string struct_param_type = param.second;
					if (struct_param_type == "int32_t") {
						if (struct_obj.HasMember(struct_param_name.c_str()) && struct_obj[struct_param_name.c_str()].IsInt()) {
							int data = struct_obj[struct_param_name.c_str()].GetInt();
							memcpy(buffer + index, (char*)&data, sizeof(int32_t));
							index = index + sizeof(int32_t);
						}
					}
					else if (struct_param_type == "double") {
						if (struct_obj.HasMember(struct_param_name.c_str()) && struct_obj[struct_param_name.c_str()].IsDouble()) {
							double data = struct_obj[struct_param_name.c_str()].GetDouble();
							memcpy(buffer + index, (char*)&data, sizeof(double));
							index = index + sizeof(double);
						}
					}
					else if (struct_param_type == "string") {
						if (struct_obj.HasMember(struct_param_name.c_str()) && struct_obj[struct_param_name.c_str()].IsString()) {
							std::string data = struct_obj[struct_param_name.c_str()].GetString();
							int len = data.length();
							char *tmp = new char[len + 1];
							strcpy(tmp, data.c_str());

							memcpy(buffer + index, (char*)tmp, len + 1);
							index = index + len + 1;
						}
					}
				}
			}
		}
	}
	else {
		cout << "fail to parse json:" << json_content << endl;
	}
	return buffer;
}


/*
bool Interface::parseConfig() {
// File Check
ifstream in("config.ini");
stringstream ss;
ss << in.rdbuf();
if (ss.fail()) {
cout << "[error] config file not found" << endl;
return false;
}
INI::Parser p(ss);
// System Name
systemId = p.top()("SYSTEM")["id"];
// Node Name
nodeName = p.top()("NODE")["name"];
cout << "[checked] <" << systemId << "> "
"node name:" << nodeName << endl;
// P & S
int pubNum = stoi(p.top()("PUBLISH")["count"]);
int subNum = stoi(p.top()("SUBSCRIBE")["count"]);
const char* pubLine = p.top()("PUBLISH")["names"].c_str();
const char* subLine = p.top()("SUBSCRIBE")["names"].c_str();
string token;
istringstream tokenStreamPub(pubLine);
while (getline(tokenStreamPub, token, ',')) {
pubNames.push_back(token);
}
istringstream tokenStreamSub(subLine);
while (getline(tokenStreamSub, token, ',')) {
subNames.push_back(token);
}
if (pubNum != pubNames.size()) {
cout << "[error] <" << systemId << "> "
<< "pub size wrong" << endl;
return false;
}
if (subNum != subNames.size()) {
cout << "[error] <" << systemId << "> "
<< "sub size wrong" << endl;
return false;
}
cout << "[checked] <" << systemId << "> "
"publish and subscribe right" << endl;
return true;
}
*/
