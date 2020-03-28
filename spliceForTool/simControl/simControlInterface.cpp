
#include "stdafx.h"
#include "simControlInterface.h"
#include <stdio.h>
#include <cstdlib>
#include <malloc.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <sstream>

using namespace std;

SimControlInterface::SimControlInterface() {
	//systemRunId = org::opensplice::domain::default_id();
}


bool SimControlInterface::simRun(string configName)
{
	try {
		SetOsplEnv();

		std::string pt("initialize_log.txt");
		CSSimLog::Instance()->CreateLog(pt);
		LogSEInfo("init log success");

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
		/*if (!startServerDDS()) {
			cout << "[error] <" << systemId << "> "
				<< "start dds fail" << endl;

			std::string msg;
			msg = systemId + "start dds fail";
			LogDDSErr(msg);
			return false;
		}

		thread th(wsServe, waitSet, systemId);
		th.detach();*/

		inst = CSDDSService::Instance();
		inst->Init(systemId);

		std::function<bool(MsgData)> cb = std::bind(&SimControlInterface::process, this, placeholders::_1);
		inst->SetCallBack(cb);
		inst->SetScenarioName(configName);

		for (auto pubName : pubNames){
			inst->CreateTopic(pubName);
			inst->CreateWriter(pubName);
		}

		for (auto subName : subNames){
			inst->CreateTopic(subName);
			inst->CreateReader(subName);
		}
		inst->StartReceiveData();

		cout << "-----CONGRATULATIONS, ALMOST DONE!-----" << endl;
		
		std::string msg;
		msg = systemId + "-----CONGRATULATIONS, ALMOST DONE!-----";
		LogDDSInfo(msg);
		// TODO 处理
		Sleep(1000);
		start_flag_ = true;
		
		MsgData data;
		data.content = "me";
		data.from = nodeName;
		data.systemId = systemId;
		data.time = currentTime;
		data.topicName = ACQUIRE_READY_STATE;
		if (inst->write(ACQUIRE_READY_STATE,data))
		{
			return true;
		}
		else{
			return false;
		}

		//return publish(ACQUIRE_READY_STATE, "me");
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

	MsgData data;
	data.content = "me";
	data.from = nodeName;
	data.systemId = systemId;
	data.time = currentTime;
	data.topicName = SIMULATION_END;
	if (inst->write(SIMULATION_END,data))
	{
		return true;
	}
	else{
		return false;
	}

	//publish(SIMULATION_END, "me");

	return true;
}

bool SimControlInterface::process(MsgData msgdata) {

	if (start_flag_ && !pause_flag_) {

		string str;
		string str_time = to_string(msgdata.time);
		string tName = msgdata.topicName;
		string from = msgdata.from;

		str = "RECEIVE <";
		str = str + tName + "> FROM <" + msgdata.from
			+ "> AT <" + str_time + ">";
		cout << str << endl;
		LogDDSInfo(str);

		string sName = msgdata.systemId;
		if (sName != systemId) {
			cout << "[error] <" << systemId << "> "
				<< "the message is not for me" << endl;

			std::string msg;
			msg = systemId + "the message is not for me";
			LogDDSErr(str);
			return false;
		}

		// prevent history data
		if ((currentTime - msgdata.time) > 10e-5) {
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

		cout << "current time: <" << currentTime << "> message time: < " << str_time << ">" << endl;

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

				MsgData data;
				data.content = "me";
				data.from = nodeName;
				data.systemId = systemId;
				data.time = currentTime;
				data.topicName = INITIAL_FEDERATE;
				inst->write(INITIAL_FEDERATE,data);

				//publish(INITIAL_FEDERATE, "me");
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

					MsgData data;
					data.content = "me";
					data.from = nodeName;
					data.systemId = systemId;
					data.time = currentTime;
					data.topicName = SIMULATION_RUN;
					inst->write(SIMULATION_RUN,data);

					//publish(SIMULATION_RUN, "me");

					pre_time_ = std::chrono::steady_clock::now();
					start_time_ = std::chrono::steady_clock::now();
					first_acquire_flag_ = false;
				}
				else
				{
					//currentTime++;
					cout << "all nodes are acquire advance" << endl;
					LogDDSInfo("all nodes are acquire advance");

					current_time_ = std::chrono::steady_clock::now();
					double diff = std::chrono::duration_cast<std::chrono::milliseconds>(current_time_ - start_time_).count();
					currentTime = diff / 1000;
					tick_count_++;

					MsgData data;
					if(currentTime > 200){
						data.content = "me";
						data.from = nodeName;
						data.systemId = systemId;
						data.time = currentTime;
						data.topicName = SIMULATION_END;
						inst->write(SIMULATION_END, data);
					}
					else{
						data.content = "me";
						data.from = nodeName;
						data.systemId = systemId;
						data.time = currentTime;
						data.topicName = ADVANCE_GRANT;
						inst->write(ADVANCE_GRANT, data);
					}
					
					

					//publish(ADVANCE_GRANT, "me");
				}
				Sleep(5000);
			}
		}
		else if (tName == SIMULATION_RUN) {

			MsgData data;
			data.content = to_string(tick_count_);
			data.from = nodeName;
			data.systemId = systemId;
			data.time = currentTime;
			data.topicName = ADVANCE_GRANT;
			inst->write(ADVANCE_GRANT,data);

			//publish(ADVANCE_GRANT, std::to_string(tick_count_));
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

bool SimControlInterface::SetOsplEnv() {
	char path[MAX_PATH];
	memset(path, 0, sizeof(path));
	if (GetModuleFileName(NULL, path, MAX_PATH)>0)
	{
		(*strrchr(path, '\\')) = '\0';//丢掉文件名，得到路径
	}

	char ospl_file_path[MAX_PATH];
	memset(ospl_file_path, 0, sizeof(ospl_file_path));
	strcat(ospl_file_path, "file://");
	strcat(ospl_file_path, path);
	strcat(ospl_file_path, "/external/OpenSplice/x64/etc/config/ospl.xml");
	errno_t er = _putenv_s("OSPL_URI", ospl_file_path);

	return true;
}