
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

bool Interface::start(initTool p_initTool, setToTool p_setToTool,
	setFinish p_setFinish, endTool p_endTool) {

	try {
		// 1.Read Config
		/*if (!parseConfig()) {
			cout << "[error] system config fail" << endl;
			return false;
		}*/

		// 1.Read Config
		if (!CSScenarioXML::getInstance().ReadXML("E:\\spliceForTool\\ZtOE0Jfu.xml")) {
			cout << "[error] system config fail" << endl;
			return false;
		}

		// 2.Parameters
		currentTime = 0.0;
		this->p_initTool = p_initTool;
		this->p_setToTool = p_setToTool;
		this->p_setFinish = p_setFinish;
		this->p_endTool = p_endTool;

		systemId = CSScenarioXML::getInstance().GetSystemId();
		nodeName = CSScenarioXML::getInstance().GetNodeName();
		
		PubSubItem pub_sub = CSScenarioXML::getInstance().GetPubSub(nodeName);
		pubNames = pub_sub.publish;
		subNames = pub_sub.subscribe;

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
		return publish(NODE_READY, "me");;
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

bool Interface::setValue(string name, string data) {
	if (!publish(name.c_str(), data)) {
		cout << "[error] <" << systemId << "> "
			<< "data sent fail at "
			<< to_string(currentTime) << endl;
		return false;
	}
	return true;
}

string Interface::ConvertTypeData2Json(string topic_name, void* data_ptr) {
	TopicDefineInfo topic_define_info = CSScenarioXML::getInstance().GetTopicDefineInfo(topic_name);
	std::unordered_map<std::string, std::string> params = topic_define_info.params;

	json json_data, json_struct;

	for (auto p : params) {
		std::string param_name = p.first;
		std::string param_type = p.second;
		if (param_type == "int32_t") {
			int data = *(int*)data_ptr;
			json_data[param_name] = data;
		}
		else if (param_type == "double") {
			double data = *(double*)data_ptr;
			json_data[param_name] = data;
		}
		else if (param_type == "string") {
			std::string data = (char*)data_ptr;
			json_data[param_name] = data;
		}
		else {
			TypeDefineInfo type_define_info = CSScenarioXML::getInstance().GetTypeDefineInfo(param_type);
			std::unordered_map<std::string, std::string> type_params = type_define_info.params;
			//void* tmp = data_ptr;
			int index = 0;
			for (auto param : type_params) {
				std::string struct_param_name = param.first;
				std::string struct_param_type = param.second;
				if (struct_param_type == "int32_t") {
					int data = *((int*)((char*)data_ptr+index));
					index = index + 8;

					json_struct[struct_param_name] = data;
				}
				else if (struct_param_type == "double") {
					double data = *((double*)((char*)data_ptr+index));
					index = index + 8;
					json_struct[struct_param_name] = data;
				}
				else if (struct_param_type == "string") {
					char buffer[1024];
					memset(buffer,0,sizeof(buffer));
					int len = strlen((char*)((char*)data_ptr + index));
					memcpy(buffer, (char*)data_ptr + index, len);
					std::string data = buffer;
					index = index + len + 1;
					json_struct[struct_param_name] = data;
				}
			}

			json_data[param_name] = json_struct;
		}
	}

	std::string str_data = json_data.dump();
	return str_data;
}

char* Interface::ConvertJson2TypeData(string topic_name, string data) {
	TopicDefineInfo topic_define_info = CSScenarioXML::getInstance().GetTopicDefineInfo(topic_name);
	std::unordered_map<std::string, std::string> params = topic_define_info.params;

	if (params.size() <= 0)
	{
		return "";
	}

	char *buffer = (char*)malloc(sizeof(char)*1024);
	memset(buffer,0,sizeof(buffer));
	int index = 0;
	json json_data = json::parse(data);

	for (auto p : params) {
		std::string param_name = p.first;
		std::string param_type = p.second;
		if (param_type == "int32_t") {
			int data = json_data[param_name];
			memcpy(buffer+index,(char*)&data, sizeof(int32_t));
			index = index + sizeof(int32_t);
		}
		else if (param_type == "double") {
			double data = json_data[param_name];
			memcpy(buffer + index, (char*)&data, sizeof(double));
			index = index + sizeof(double);
		}
		else if (param_type == "string") {
			std::string data = json_data[param_name];
			int len = data.length();
			char *tmp = new char[len + 1];
			strcpy(tmp, data.c_str());

			memcpy(buffer + index, (char*)&data, len+1);
			index = index + len +1;
		}
		else {
			TypeDefineInfo type_define_info = CSScenarioXML::getInstance().GetTypeDefineInfo(param_type);
			std::unordered_map<std::string, std::string> type_params = type_define_info.params;

			json json_struct = json_data[param_name];

			for (auto param : type_params) {
				std::string struct_param_name = param.first;
				std::string struct_param_type = param.second;
				if (struct_param_type == "int32_t") {
					int data = json_struct[struct_param_name];
					memcpy(buffer + index, (char*)&data, sizeof(int32_t));
					index = index + sizeof(int32_t);
				}
				else if (struct_param_type == "double") {
					double data = json_struct[struct_param_name];
					memcpy(buffer + index, (char*)&data, sizeof(double));
					index = index + sizeof(double);
				}
				else if (struct_param_type == "string") {
					std::string data = json_struct[struct_param_name];
					int len = data.length();
					char *tmp = new char[len + 1];
					strcpy(tmp, data.c_str());

					memcpy(buffer + index, (char*)&data, len + 1);
					index = index + len + 1;
				}
			}
		}
	}

	return buffer;
}

bool Interface::setValue(string topic_name, void* data_ptr) {
	string  data = ConvertTypeData2Json(topic_name, data_ptr);
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

			char* datac = ConvertJson2TypeData(key,value);
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

bool Interface::parseConfig() {

	// File Check
	ifstream in("E:\\spliceForTool\\config.ini");
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
