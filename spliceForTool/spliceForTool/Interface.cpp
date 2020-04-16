
#include "stdafx.h"
#include "Interface.h"
#include <chrono>
#include <ctime>

Interface::Interface() {
	p_XMLapi = new CSScenarioXML();
	p_JSONapi = new JSONapi(p_XMLapi);
}

Interface::~Interface() {
}

Interface* Interface::Instance() {
	static Interface service;
	return &service;
}

std::string TimePointToString(std::chrono::time_point<std::chrono::system_clock>
	time_point) {
	std::time_t t = std::chrono::system_clock::to_time_t(time_point);
	std::tm tm2;
	localtime_s(&tm2, &t);
	char buf[100] = { 0 };
	std::strftime(buf, sizeof(buf), "%Y%m%d%H%M%S", &tm2);
	std::string str = buf;
	return std::move(str);
}

string Interface::start(string configName,
	initTool p_initTool, setToTool p_setToTool,
	setFinish p_setFinish, endTool p_endTool) {

	SetOsplEnv();

	string pt("initialize_log.txt");
	CSSimLog::Instance()->CreateLog(pt);
	LogSEInfo("init log success");

	string msg;

	try {
		// 1.Read Config
		if (!p_XMLapi->ReadXML(configName)) {
			LogSEErr("parse xml file fail");
			return "";
		}
		systemId = p_XMLapi->GetSystemId();
		nodeName = p_XMLapi->GetNodeName();
		PubSubItem pub_sub = p_XMLapi->GetPubSub(nodeName);
		pubNames = pub_sub.publish;
		subNames = pub_sub.subscribe;

		// 2.Log
		ifstream infile;
		infile.open("initialize_log.txt");
		if (!infile.is_open()) {
			LogSEErr("open initialize log file fail");
		}

		string csscenario_full_logname = nodeName + TimePointToString(std::chrono::system_clock::now()) + ".txt";
		ofstream outfile;
		outfile.open(csscenario_full_logname);
		if (!outfile.is_open()) {
			string msg = "open" + csscenario_full_logname + "fail";
			LogSEErr(msg);
		}

		CSSimLog::Instance()->CloseLog();

		string line;
		while (getline(infile, line)) {
			outfile << line << endl;
		}
		infile.close();
		outfile.close();
		CSSimLog::Instance()->CreateLog(csscenario_full_logname);
		if (std::remove("initialize_log.txt") == -1) {
			LogSEErr("delete initialize log fail");
		}

		// 3.Parameters
		currentTime = 0.0;
		this->p_initTool = p_initTool;
		this->p_setToTool = p_setToTool;
		this->p_setFinish = p_setFinish;
		this->p_endTool = p_endTool;
		LogSEInfo("set call back successed!");

		// 4.DDS
		p_ddsInst = CSDDSService::Instance();
		p_ddsInst->Init(systemId);

		for (auto pubName : pubNames){
			p_ddsInst->CreateTopic(pubName);
			p_ddsInst->CreateWriter(pubName);
		}

		for (auto subName : subNames){
			p_ddsInst->CreateTopic(subName);
			p_ddsInst->CreateReader(subName);
		}

		function<bool(MsgData)> cb = bind(&Interface::process, this, placeholders::_1);
		p_ddsInst->SetCallBack(cb);
		p_ddsInst->SetScenarioName(nodeName);
		p_ddsInst->InitDataBase();
		p_ddsInst->StartReceiveData();
		LogSEInfo("start dds successed!");
		// TODO 处理
		Sleep(1000);
		if (publish(NODE_READY, "me")) {
			return systemId;
		}
		else{
			return "";
		}
	}
	catch (runtime_error& e) {
		msg = systemId + " runtime : " + e.what();
		LogSEErr(msg);
		return "";
	}
	catch (exception &e) {
		msg = systemId + " exception : " + e.what();
		LogSEErr(msg);
		return "";
	}
}

bool Interface::setValue(string topic_name, void* data_ptr) {
	
	string data = p_JSONapi->ConvertTypeData2Json(topic_name, data_ptr);
	string msg;

	if (!publish(topic_name, data)) {
		 msg = systemId + " data send fail at " + to_string(currentTime);
		 LogSEErr(msg);
		return false;
	}
	msg = systemId + " data send successed at " + to_string(currentTime);
	// LogSEInfo(msg);
	return true;
}

bool Interface::advance() {
	string msg;
	if (!publish(ADVANCE_REQUEST, to_string(currentTime))) {
		msg = systemId + " advance send fail at " + to_string(currentTime);
		LogDDSErr(msg);
		return false;
	}
	msg = systemId + " advance send successed at " + to_string(currentTime);
	// LogSEInfo(msg);
	return true;
}

bool Interface::end() {
	p_ddsInst->StopReceiveData();
	LogSEInfo("end tool");
	return true;
}

bool Interface::stopDDS() {
	p_ddsInst->Clear();
	LogSEInfo("清理dds完成");
	CSSimLog::Instance()->CloseLog();
	return true;
}

bool Interface::process(const MsgData& msgdata) {

#ifdef STDOUTTEST
	
	std::cout << "=======receive data:===========" << endl;
	std::cout << "*********************" << endl;
	std::cout << "content:" << msgdata.content << endl;
	std::cout << "from:" << msgdata.from << endl;
	std::cout << "subjectId:" << msgdata.subjectId << endl;
	std::cout << "systemId:" << msgdata.systemId << endl;
	std::cout << "time:" << msgdata.time << endl;
	std::cout << "topicName:" << msgdata.topicName << endl;
	std::cout << "*********************" << endl;
	
#endif

	string msg;
	string str;
	string str_time = to_string(msgdata.time);
	string tName = msgdata.topicName;

	string sName = msgdata.systemId;
	if (sName != systemId) {
		msg = systemId + " the message is not for me";
		LogSEErr(msg);
		return false;
	}

	// prevent history data
	if ((currentTime - msgdata.time) > 10e-5) {
		str = "Old Data {";
		str = str + str_time + "} at {"
			+ to_string(currentTime) + "}";
		
		msg = systemId + str;
		LogSEErr(msg);
		return false;
	}

	if (tName == ACQUIRE_READY_STATE) {
		if (!publish(NODE_READY, to_string(currentTime))) {
			msg = systemId + " data send fail at " + to_string(currentTime);
			LogSEErr(msg);
			return false;
		}
		LogSEInfo("publish node ready");
	}
	else if (tName == INITIAL_FEDERATE) {
		currentTime = msgdata.time;

		stringstream ss;
		ss << msgdata.content;
		ss >> step;

		(*p_initTool)(currentTime, step);
		LogSEInfo("init tool");
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

			char* datac = p_JSONapi->ConvertJson2TypeData(key, value);

			(*p_setToTool)(currentTime, cstr, (void*)datac);
			delete[] cstr;
			delete[] cstr2;
			iter++;
		}
		currentTime = msgdata.time;
		(*p_setFinish)(currentTime);
		// LogSEInfo("call tool finish funtion done");
	}
	else if (tName == SIMULATION_RUN) {
		// TODO
		advance();
	}
	else if (tName == SIMULATION_END) {
		// TODO
		(*p_endTool)();
		LogSEInfo("call end interface");
	}
	else {
		// actual data
		if (abs(msgdata.time - currentTime) < 10e-5) {
			dataMap.insert(make_pair(msgdata.topicName, msgdata.content));
		}
		else {
			backupDataMap.insert(make_pair(msgdata.topicName, msgdata.content));
		}
	}
	return true;
}

bool Interface::publish(string topic, string data) {
	MsgData msgdata;
	msgdata.from = nodeName;
	msgdata.systemId = systemId;
	msgdata.time = currentTime;
	msgdata.topicName = topic;
	msgdata.content = data;
	return p_ddsInst->write(topic, msgdata);
}

bool Interface::SetOsplEnv() {
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
	strcat(ospl_file_path, "/external/OpenSplice/etc/config/ospl.xml");
	errno_t er = _putenv_s("OSPL_URI", ospl_file_path);

	return true;
}
