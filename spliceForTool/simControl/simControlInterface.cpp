
#include "stdafx.h"
#include "simControlInterface.h"
#include <stdio.h>
#include <cstdlib>
#include <malloc.h>
#include <iostream>
#include <fstream>
#include <cassert>

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
				SimControlInterface::getInstance().process(sample->data());
			}
		}
	}
private:
	dds::sub::status::DataState& dataState;
};

bool wsServe(dds::core::cond::WaitSet waitSet, string systemId) {
	cout << "[checked] <" << systemId << "> "
		<< "dds detached thread starts well" << endl;
	std::string msg;
	msg = systemId + "dds detached thread starts well";
	LogDDSInfo(msg);

	while (1) {
		try {
			waitSet.dispatch();
		}
		catch (const dds::core::TimeoutError e) {
			cout << "[error] <" << systemId << "> "
				"dds thread:" << e.what() << endl;
			std::string msg;
			msg = systemId + "dds thread:" + e.what();
			LogDDSErr(msg);
			return false;
		}
	}
	cout << "[checked] <" << systemId << "> "
		<< "dds detached thread ends" << endl;
	msg = systemId + "dds detached thread ends";
	LogDDSInfo(msg);
	return true;
}

SimControlInterface::SimControlInterface() {
	systemRunId = org::opensplice::domain::default_id();

	std::string pt("initialize_log.txt");
	CSSimLog::Instance()->CreateLog(pt);
	LogDDSInfo("init log success");
}


bool SimControlInterface::simRun(string configName)
{
	try {
		// 1.Read Config
		if (!xml_parser_.ReadXML(configName)) {
			cout << "[error] system config fail" << endl;

			LogDDSErr("parse xml file fail");
			return false;
		}

		systemId = xml_parser_.GetSystemId();
		nodeName = xml_parser_.GetNodeName();

		PubSubItem pub_sub = xml_parser_.GetPubSub(nodeName);
		pubNames = pub_sub.publish;
		subNames = pub_sub.subscribe;

		//log
		ifstream infile;
		infile.open("initialize_log.txt");
		if (!infile.is_open())
		{
			LogDDSErr("open initialize log file fail");
		}

		std::string csscenario_full_logname = nodeName + ".txt";
		ofstream outfile;
		outfile.open(csscenario_full_logname);
		if (!outfile.is_open())
		{
			std::string msg = "open" + csscenario_full_logname + "fail";
			LogDDSErr(msg);
		}

		CSSimLog::Instance()->CloseLog();

		std::string line;
		while (getline(infile, line)) {
			outfile << line << endl;
		}
		infile.close();
		outfile.close();
		CSSimLog::Instance()->CreateLog(csscenario_full_logname);
		if (std::remove("initialize_log.txt") == -1) {
			LogDDSInfo("delete initialize log fail");
		}

		//2.node
		std::vector<std::string>allNodes = xml_parser_.GetAllNode();
		for (auto node : allNodes)
		{
			if (node != nodeName) {
				ready_state_[node] = false;
				advance_request_state_[node] = false;
			}
		}

		// 3.DDS
		if (!startServerDDS()) {
			cout << "[error] <" << systemId << "> "
				<< "start dds fail" << endl;

			std::string msg;
			msg = systemId + "start dds fail";
			LogDDSErr(msg);
			return false;
		}

		thread th(wsServe, waitSet, systemId);
		th.detach();

		cout << "-----CONGRATULATIONS, ALMOST DONE!-----" << endl;
		
		std::string msg;
		msg = systemId + "-----CONGRATULATIONS, ALMOST DONE!-----";
		LogDDSInfo(msg);
		// TODO 处理
		Sleep(1000);
		start_flag_ = true;
		
		return publish(ACQUIRE_READY_STATE, "me");
	}
	catch (runtime_error& e) {
		cout << "[error] <" << systemId << "> "
			<< "runtime:" << e.what() << endl;

		std::string msg;
		msg = systemId + "runtime:" + e.what();
		LogDDSErr(msg);
		return false;
	}
	catch (exception &e) {
		cout << "[exception] <" << systemId << "> "
			<< e.what() << endl;

		std::string msg;
		msg = systemId + "exception:" + e.what();
		LogDDSErr(msg);
		return false;
	}
}

bool SimControlInterface::simPause()
{
	pause_flag_ = true;
	return true;
}

bool SimControlInterface::simContinue()
{
	pause_flag_ = false;
	return true;
}

bool SimControlInterface::simEnd()
{
	start_flag_ = false;
	publish(SIMULATION_END, "me");

	return true;
}

bool SimControlInterface::process(Msg messageIn) {

	if (start_flag_ && !pause_flag_) {

		string str;
		string str_time = to_string(messageIn.time());
		string tName = messageIn.topicName();
		string from = messageIn.from();

		str = "RECEIVE <";
		str = str + tName + "> FROM <" + messageIn.from()
			+ "> AT <" + str_time + ">";
		cout << str << endl;
		LogDDSInfo(str);

		string sName = messageIn.systemId();
		if (sName != systemId) {
			cout << "[error] <" << systemId << "> "
				<< "the message is not for me" << endl;

			std::string msg;
			msg = systemId + "the message is not for me";
			LogDDSErr(str);
			return false;
		}

		// prevent history data
		if ((currentTime - messageIn.time()) > 10e-5) {
			str = "Old Data {";
			str = str + str_time + "} at {"
				+ to_string(currentTime) + "}";
			cout << "[error] <" << systemId << "> "
				<< str << endl;

			std::string msg;
			msg = systemId + str;
			LogDDSInfo(msg);
			return false;
		}

		cout << "current time: <" << currentTime << "> message time: < "<< str_time << ">"<< endl;

		if (tName == NODE_READY) {
			//检测各节点准备状态，发送初始化主题；
			ready_state_[from] = true;
			cout << "i have accept <" << from << "> is ready" << endl;

			std::string msg;
			msg = "i have accept <" + from + "> is ready";
			LogDDSInfo(msg);

			if (CheckAllNodeReadyState())
			{
				cout << "all node is ready,start init" << endl;
				LogDDSInfo("all node is ready,start init");
				publish(INITIAL_FEDERATE, "me");
			}
		}
		else if (tName == ADVANCE_REQUEST) {
			mx_.lock();
			advance_request_state_[from] = true;
			mx_.unlock();
			if (CheckAllNodeAdvanceRequestState())
			{
				ResetAllNodeAdvanceRequestState();

				if (first_acquire_flag_)
				{
					cout << "all nodes are init,start run" << endl;
					LogDDSInfo("all nodes are init,start run");

					publish(SIMULATION_RUN, "me");
					pre_time_ = std::chrono::steady_clock::now();
					start_time_ = std::chrono::steady_clock::now();
					first_acquire_flag_ = false;
				}
				else
				{
					//currentTime++;
					cout << "all nodes are acquire advance" << endl;
					LogDDSInfo("all nodes are acquire advance");
					publish(ADVANCE_GRANT, "me");
				}
				Sleep(10000);
			}
		}
		else if (tName == SIMULATION_RUN) {
			publish(ADVANCE_GRANT, std::to_string(tick_count_));
		}
		else {
			// actual data
	
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool SimControlInterface::startServerDDS() {

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

bool SimControlInterface::publish(string topic, string data) {
	random_device rd;
	mt19937 mt(rd());

	if (topic == "advance_grant")
	{
		//currentTime++;

		current_time_ = std::chrono::steady_clock::now();
		double diff = std::chrono::duration_cast<std::chrono::milliseconds>
			(current_time_ - start_time_).count();
		currentTime = diff / 1000;
		tick_count_++;
	}

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

string SimControlInterface::ConvertTypeData2Json(string topic_name, void* data_ptr) {

	TopicDefineInfo topic_define_info = xml_parser_.GetTopicDefineInfo(topic_name);
	std::unordered_map<std::string, std::string> params = topic_define_info.params;

	rapidjson::StringBuffer buf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
	writer.StartObject();
	//json json_data, json_struct;

	for (auto p : params) {
		std::string param_name = p.first;
		std::string param_type = p.second;
		if (param_type == "int32_t") {
			int data = *(int*)data_ptr;
			writer.Key(param_name.c_str()); writer.Int(data);
			//json_data[param_name] = data;
		}
		else if (param_type == "double") {
			double data = *(double*)data_ptr;
			writer.Key(param_name.c_str()); writer.Double(data);
			//json_data[param_name] = data;
		}
		else if (param_type == "string") {
			std::string data = (char*)data_ptr;
			writer.Key(param_name.c_str()); writer.String(data.c_str());
			//json_data[param_name] = data;
		}
		else {
			TypeDefineInfo type_define_info = xml_parser_.GetTypeDefineInfo(param_type);
			std::unordered_map<std::string, std::string> type_params = type_define_info.params;
			//void* tmp = data_ptr;
			writer.Key(param_name.c_str());
			writer.StartObject();
			int index = 0;
			for (auto param : type_params) {
				std::string struct_param_name = param.first;
				std::string struct_param_type = param.second;
				if (struct_param_type == "int32_t") {
					int data = *((int*)((char*)data_ptr + index));
					index = index + 8;
					writer.Key(struct_param_name.c_str()); writer.Int(data);
					//json_struct[struct_param_name] = data;
				}
				else if (struct_param_type == "double") {
					double data = *((double*)((char*)data_ptr + index));
					index = index + 8;
					writer.Key(struct_param_name.c_str()); writer.Double(data);
					//json_struct[struct_param_name] = data;
				}
				else if (struct_param_type == "string") {
					char buffer[1024];
					memset(buffer, 0, sizeof(buffer));
					int len = strlen((char*)((char*)data_ptr + index));
					memcpy(buffer, (char*)data_ptr + index, len);
					std::string data = buffer;
					index = index + len + 1;
					writer.Key(param_name.c_str()); writer.String(data.c_str());
					//json_struct[struct_param_name] = data;
				}
			}
			writer.EndObject();
			//json_data[param_name] = json_struct;
		}
	}
	writer.EndObject();
	const char* json_content = buf.GetString();
	fprintf(stdout, "json content: %s\n", json_content);
	std::string str_data = json_content;

	std::string msg = "json content :" + str_data;
	LogDDSInfo(msg);
	return str_data;

	//std::string str_data = json_data.dump();
	//return str_data;
}

char* SimControlInterface::ConvertJson2TypeData(string topic_name, string data) {

	TopicDefineInfo topic_define_info = xml_parser_.GetTopicDefineInfo(topic_name);
	std::unordered_map<std::string, std::string> params = topic_define_info.params;

	char *buffer = (char*)malloc(sizeof(char) * 1024);
	memset(buffer, 0, sizeof(buffer));
	int index = 0;

	if (params.size() <= 0) {
		return buffer;
	}

	//json json_data = json::parse(data);

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

		std::string msg;
		msg = "fail to parse json:" + json_content;
		LogDDSErr(msg);
	}

	return buffer;
}

bool SimControlInterface::CheckAllNodeReadyState(){
	for (auto node : ready_state_)
	{
		if (node.second)
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	return true;
}

//检测各节点步进请求状态
bool SimControlInterface::CheckAllNodeAdvanceRequestState()
{
	mx_.lock();
	for (auto node : advance_request_state_)
	{
		if (node.second)
		{
			continue;
		}
		else
		{
			mx_.unlock();
			return false;
		}
	}
	mx_.unlock();
	return true;
}
//重置各节点步进请求状态
bool SimControlInterface::ResetAllNodeAdvanceRequestState()
{
	mx_.lock();
	for (auto& node : advance_request_state_)
	{
		node.second = false;
	}
	mx_.unlock();
	return true;
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
