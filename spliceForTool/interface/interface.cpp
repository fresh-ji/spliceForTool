
#include "Interface.h"

Interface::Interface() {
	try {
		p_XMLapi = new CSScenarioXML();
		p_JSONapi = new JSONapi(p_XMLapi);
		isInitial = false;
	}
	catch (exception &e) {
		string er = e.what();
		LogDDSErr("Interface::Interface " + er);
	}
	catch (...) {
		LogDDSCri("Interface::Interface CATCHED!!");
	}
}

Interface* Interface::Instance() {
	try {
		static Interface service;
		return &service;
	}
	catch (exception &e) {
		string er = e.what();
		LogDDSErr("Interface::Instance " + er);
		return NULL;
	}
	catch (...) {
		LogDDSCri("Interface::Instance CATCHED!!");
		return NULL;
	}
}

string Interface::start(string configName,
	initTool p_initTool, setToTool p_setToTool,
	setFinish p_setFinish, endTool p_endTool) {
	try {
		// 1.Read Config
		if (!p_XMLapi->ReadXML(configName)) {
			LogDDSErr("parse xml file fail");
			return "";
		}

		systemId = p_XMLapi->GetSystemId();
		nodeName = p_XMLapi->GetNodeName();
		PubSubItem pub_sub = p_XMLapi->GetPubSub(nodeName);
		pubNames = pub_sub.publish;
		subNames = pub_sub.subscribe;

		// 2.Log
		string csscenario_full_logname = nodeName + ".txt";
		CSSimLog::Instance()->CreateLog(csscenario_full_logname);
		LogDDSInfo("created log");

		// 2.5.env
		SetOsplEnv();
		LogDDSInfo("set env");

		// 3.Parameters
		currentTime = 0.0;
		this->p_initTool = p_initTool;
		this->p_setToTool = p_setToTool;
		this->p_setFinish = p_setFinish;
		this->p_endTool = p_endTool;

		// 4.DDS
		p_ddsInst = CSDDSService::Instance();
		p_ddsInst->Init(systemId);
		LogDDSInfo("initialed dds");
		
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
		p_ddsInst->StartReceiveData();
		LogDDSInfo("start dds successed!");

		// TODO 处理
		Sleep(1200);
		if (publish(NODE_READY, "me")) {
			return systemId;
		}
		else{
			return "";
		}
	}
	catch (exception &e) {
		string er = e.what();
		LogDDSErr("Interface::start " + er);
		return "";
	}
	catch (...) {
		LogDDSCri("Interface::start CATCHED!!");
		return "";
	}
}

bool Interface::setValue(string topic_name, void* data_ptr) {
	try {
		string data = p_JSONapi->ConvertTypeData2Json(topic_name, data_ptr);

		if (!publish(topic_name, data)) {
			string msg = systemId + " data send fail at " + to_string(currentTime);
			LogDDSErr(msg);
			return false;
		}
		return true;
	}
	catch (exception &e) {
		string er = e.what();
		LogDDSErr("Interface::setValue " + er);
		return false;
	}
	catch (...) {
		LogDDSCri("Interface::setValue CATCHED!!");
		return false;
	}
}

bool Interface::advance() {
	try {
		if (!publish(ADVANCE_REQUEST, to_string(currentTime))) {
			string msg = systemId + " advance send fail at " + to_string(currentTime);
			LogDDSErr(msg);
			return false;
		}
		return true;
	}
	catch (exception &e) {
		string er = e.what();
		LogDDSErr("Interface::advance " + er);
		return false;
	}
	catch (...) {
		LogDDSCri("Interface::advance CATCHED!!");
		return false;
	}
}

bool Interface::end() {
	try {
		if (!publish(NODE_END, "die")) {
			string msg = systemId + " request end " + to_string(currentTime);
			LogDDSErr(msg);
			return false;
		}
		return true;
	}
	catch (exception &e) {
		string er = e.what();
		LogDDSErr("Interface::end " + er);
		return false;
	}
	catch (...) {
		LogDDSCri("Interface::end CATCHED!!");
		return false;
	}
}

bool Interface::process(const MsgData& msgdata) {
	try {
#ifdef STDOUTTRACE
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
			LogDDSErr(msg);
			return false;
		}

		// prevent history data
		if ((currentTime - msgdata.time) > 10e-5) {
			str = "Old Data {";
			str = str + str_time + "} at {"
				+ to_string(currentTime) + "}";

			msg = systemId + str;
			LogDDSErr(msg);
			return false;
		}

		if (tName == ACQUIRE_READY_STATE) {
			if (!publish(NODE_READY, to_string(currentTime))) {
				msg = systemId + " data send fail at " + to_string(currentTime);
				LogDDSErr(msg);
				return false;
			}
		}
		else if (tName == INITIAL_FEDERATE) {
			currentTime = msgdata.time;

			stringstream ss;
			ss << msgdata.content;
			ss >> step;

			(*p_initTool)(currentTime, step);

			isInitial = true;
		}
		else if (tName == ADVANCE_GRANT) {
			if (!isInitial) {
				p_ddsInst->StopReceiveData();
				(*p_endTool)();
				return false;
			}
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
		}
		else if (tName == DUPLICATED_ONLINE) {
			if (nodeName == msgdata.content) {
				p_ddsInst->StopReceiveData();
				(*p_endTool)();
			}
		}
		else if (tName == SIMULATION_END) {
			p_ddsInst->StopReceiveData();
			(*p_endTool)();
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
	catch (exception &e) {
		string er = e.what();
		LogDDSErr("Interface::process " + er);
		return false;
	}
	catch (...) {
		LogDDSCri("Interface::process CATCHED!!");
		return false;
	}
}

bool Interface::publish(string topic, string data) {
	try {
		MsgData msgdata;
		msgdata.from = nodeName;
		msgdata.systemId = systemId;
		msgdata.time = currentTime;
		msgdata.topicName = topic;
		msgdata.content = data;
		return p_ddsInst->write(topic, msgdata);
	}
	catch (exception &e) {
		string er = e.what();
		LogDDSErr("Interface::publish " + er);
		return false;
	}
	catch (...) {
		LogDDSCri("Interface::publish CATCHED!!");
		return false;
	}
}

bool Interface::SetOsplEnv() {
	try {
		char* pathvar = getenv("LARKSIMU_HOME");

		char ospl_file_path[MAX_PATH];
		memset(ospl_file_path, 0, sizeof(ospl_file_path));
		strcat(ospl_file_path, "file://");
		strcat(ospl_file_path, pathvar);
		strcat(ospl_file_path, "/external/OpenSplice/etc/config/ospl.xml");
		errno_t er = _putenv_s("OSPL_URI", ospl_file_path);

		char ospl_file_path2[MAX_PATH];
		memset(ospl_file_path2, 0, sizeof(ospl_file_path2));
		strcat(ospl_file_path2, pathvar);
		strcat(ospl_file_path2, "/lark_simu_ospl_log/");
		errno_t er2 = _putenv_s("OSPL_LOGPATH", ospl_file_path2);

		return true;
	}
	catch (exception &e) {
		string er = e.what();
		LogDDSErr("Interface::publish " + er);
		return false;
	}
	catch (...) {
		LogDDSCri("Interface::publish CATCHED!!");
		return false;
	}
}
