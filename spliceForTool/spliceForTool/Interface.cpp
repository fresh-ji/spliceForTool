
#include "stdafx.h"
#include "Interface.h"
#include <iostream>
#include <sstream>

using namespace std;
//bool wsServe(dds::core::cond::WaitSet waitSet, string systemId) {
//
//#ifdef STDOUTTEST
//	cout << "[info] <" << systemId << "> "
//		<< "dds detached thread starts" << endl;
//#endif
//	string msg;
//	msg = systemId + " dds detached thread starts";
//	// TODO LogDDSInfo(msg);
//	while (1) {
//		try {
//			waitSet.dispatch();
//		}
//		catch (const dds::core::TimeoutError e) {
//
//#ifdef STDOUTTEST
//			cout << "[error] <" << systemId << "> "
//				"dds thread:" << e.what() << endl;
//#endif
//			msg = systemId + " dds thread : " + e.what();
//			// TODO LogDDSErr(msg);
//			return false;
//		}
//		catch (runtime_error& e) {
//
//#ifdef STDOUTTEST
//			cout << "[error] <" << systemId << "> "
//				"dds thread:" << e.what() << endl;
//#endif
//			msg = systemId + " dds thread : " + e.what();
//			// TODO LogDDSErr(msg);
//			return false;
//		}
//		catch (exception &e) {
//
//#ifdef STDOUTTEST
//			cout << "[exception] <" << systemId << "> "
//				"dds thread:" << e.what() << endl;
//#endif
//			msg = systemId + " dds thread : " + e.what();
//			// TODO LogDDSErr(msg);
//			return false;
//		}
//	}
//
//#ifdef STDOUTTEST
//	cout << "[info] <" << systemId << "> "
//		<< "dds detached thread ends" << endl;
//#endif
//	msg = systemId + " dds detached thread ends";
//	// TODO LogDDSInfo(msg);
//	return true;
//}

Interface::Interface() {
	// TODO 多实例配置
	//systemRunId = org::opensplice::domain::default_id();

	p_XMLapi = new CSScenarioXML();
	p_JSONapi = new JSONapi(p_XMLapi);

	std::string pt("initialize_log.txt");
	CSSimLog::Instance()->CreateLog(pt);
	LogSEInfo("init log success");
}

string Interface::start(string configName,
	initTool p_initTool, setToTool p_setToTool,
	setFinish p_setFinish, endTool p_endTool) {

	string msg;

	try {
		// 1.Read Config
		if (!p_XMLapi->ReadXML(configName)) {
			// TODO LogDDSErr("parse xml file fail");
			return "";
		}

		systemId = p_XMLapi->GetSystemId();
		nodeName = p_XMLapi->GetNodeName();
		PubSubItem pub_sub = p_XMLapi->GetPubSub(nodeName);
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

		// 2.Parameters
		currentTime = 0.0;
		this->p_initTool = p_initTool;
		this->p_setToTool = p_setToTool;
		this->p_setFinish = p_setFinish;
		this->p_endTool = p_endTool;

		// 3.DDS
//		p_DDSapi = new DDSapi(this);
//		if (!p_DDSapi->startServerDDS()) {
//
//#ifdef STDOUTTEST
//			cout << "[error] <" << systemId << "> "
//				<< "starts dds fail" << endl;
//#endif
//			msg = systemId + " starts dds fail";
//			// TODO LogDDSErr(msg);
//			return "";
//		}
//
//		thread th(wsServe, p_DDSapi->waitSet, systemId);
//		th.detach();

		inst = CSDDSService::Instance();
		inst->Init("dds");

		for (auto pubName : pubNames){
			inst->CreateTopic(pubName);
			inst->CreateWriter(pubName);
		}

		for (auto subName : subNames){
			inst->CreateTopic(subName);
			inst->CreateReader(subName);
		}

		std::function<bool(MsgData)> cb = std::bind(&Interface::process, this, placeholders::_1);
		inst->SetCallBack(cb);

		inst->StartReceiveData();
		

#ifdef STDOUTTEST
		cout << "-----CONGRATULATIONS, ALMOST DONE!-----" << endl;
#endif
		// TODO 处理
		Sleep(1000);
		MsgData data;
		data.content = "me";
		data.from = nodeName;
		data.systemId = systemId;
		data.time = currentTime;
		data.topicName = NODE_READY;
		if (inst->write(NODE_READY,data))
		{
			return systemId;
		}
		else{
			return "";
		}

		/*if (p_DDSapi->publish(NODE_READY, "me")) {
			return systemId;
		}
		else {
			return "";
		}*/
	}
	catch (runtime_error& e) {

#ifdef STDOUTTEST
		cout << "[error] <" << systemId << "> "
			<< "runtime:" << e.what() << endl;
#endif
		msg = systemId + " runtime : " + e.what();
		// TODO LogDDSErr(msg);
		return "";
	}
	catch (exception &e) {

#ifdef STDOUTTEST
		cout << "[exception] <" << systemId << "> "
			<< e.what() << endl;
#endif
		msg = systemId + " exception : " + e.what();
		// TODO LogDDSErr(msg);
		return "";
	}
}

bool Interface::setValue(string topic_name, void* data_ptr) {
	string data = p_JSONapi->ConvertTypeData2Json(topic_name, data_ptr);

	MsgData msgdata;
	msgdata.content = data;
	msgdata.from = nodeName;
	msgdata.systemId = systemId;
	msgdata.time = currentTime;
	msgdata.topicName = topic_name;
	if (!inst->write(topic_name,msgdata))
	{
		string msg;
		msg = systemId + " data send fail at " + to_string(currentTime);
		// TODO LogDDSErr(msg);
		return false;
	}
	
	//if (!p_DDSapi->publish(topic_name.c_str(), data)) {
	//	string msg;
	//	msg = systemId + " data send fail at " + to_string(currentTime);
	//	// TODO LogDDSErr(msg);
	//	return false;
	//}

	return true;
}

bool Interface::advance() {
	MsgData msgdata;
	msgdata.content = currentTime;
	msgdata.from = nodeName;
	msgdata.systemId = systemId;
	msgdata.time = currentTime;
	msgdata.topicName = ADVANCE_REQUEST;

	if (!inst->write(ADVANCE_REQUEST,msgdata))
	{
		string msg;
		msg = systemId + " advance send fail at " + to_string(currentTime);
		// TODO LogDDSErr(msg);
		return false;
	}

	//if (!p_DDSapi->publish(ADVANCE_REQUEST, to_string(currentTime))) {
	//	string msg;
	//	msg = systemId + " advance send fail at " + to_string(currentTime);
	//	// TODO LogDDSErr(msg);
	//	return false;
	//}
#ifdef STDOUTTEST
	cout << "<" << systemId << "> advance sent at "
		<< to_string(currentTime) << endl;
#endif
	return true;
}

bool Interface::end() {
	// TODO
	return true;
}

//bool Interface::process(Msg messageIn) {
//
//	string str;
//	string str_time = to_string(messageIn.time());
//	string tName = messageIn.topicName();
//
//#ifdef STDOUTTEST
//	str = "RECEIVE <";
//	str = str + tName + "> FROM <" + messageIn.from()
//		+ "> AT <" + str_time + ">";
//	cout << str << endl;
//#endif
//
//	string sName = messageIn.systemId();
//	if (sName != systemId) {
//		string msg;
//		msg = systemId + " the message is not for me";
//		// TODO LogDDSInfo(msg);
//		return false;
//	}
//
//	// prevent history data
//	if ((currentTime - messageIn.time()) > 10e-5) {
//		str = "Old Data {";
//		str = str + str_time + "} at {"
//			+ to_string(currentTime) + "}";
//		string msg;
//		msg = systemId + str;
//		// TODO LogDDSInfo(msg);
//		return false;
//	}
//
//	if (tName == ACQUIRE_READY_STATE) {
//		p_DDSapi->publish(NODE_READY, "me");
//	}
//	else if (tName == INITIAL_FEDERATE) {
//		currentTime = messageIn.time();
//
//		stringstream ss;
//		ss << messageIn.content();
//		ss >> step;
//
//		(*p_initTool)(currentTime, step);
//	}
//	else if (tName == ADVANCE_GRANT) {
//		map<string, string> tempMap = dataMap;
//		dataMap = backupDataMap;
//		backupDataMap.clear();
//
//		map<string, string>::iterator iter;
//		iter = tempMap.begin();
//		while (iter != tempMap.end()) {
//			string key = iter->first;
//			char *cstr = new char[key.length() + 1];
//			strcpy(cstr, key.c_str());
//			string value = iter->second;
//			char *cstr2 = new char[value.length() + 1];
//			strcpy(cstr2, value.c_str());
//
//			char* datac = p_JSONapi->ConvertJson2TypeData(key, value);
//
//			(*p_setToTool)(currentTime, cstr, (void*)datac);
//			delete[] cstr;
//			delete[] cstr2;
//			iter++;
//		}
//		currentTime = messageIn.time();
//		(*p_setFinish)(currentTime);
//	}
//	else if (tName == SIMULATION_END) {
//		// TODO
//		(*p_endTool)();
//	}
//	else {
//		// actual data
//		if (abs(messageIn.time() - currentTime) < 10e-5) {
//			dataMap.insert(make_pair(messageIn.topicName(), messageIn.content()));
//		}
//		else {
//			backupDataMap.insert(make_pair(messageIn.topicName(), messageIn.content()));
//		}
//	}
//	return true;
//}


bool Interface::process(MsgData msgdata)
{
	/*std::cout << "=======receive data:===========" << endl;
	std::cout << "*********************" << endl;
	std::cout << "content:" << msgdata.content << endl;
	std::cout << "from:" << msgdata.from << endl;
	std::cout << "subjectId:" << msgdata.subjectId << endl;
	std::cout << "systemId:" << msgdata.systemId << endl;
	std::cout << "time:" << msgdata.time << endl;
	std::cout << "topicName:" << msgdata.topicName << endl;
	std::cout << "*********************" << endl;*/

	string str;
	string str_time = to_string(msgdata.time);
	string tName = msgdata.topicName;

#ifdef STDOUTTEST
	str = "RECEIVE <";
	str = str + tName + "> FROM <" + msgdata.from + "> AT <" + str_time + ">";
	cout << str << endl;
#endif

	string sName = msgdata.systemId;
	if (sName != systemId) {
		string msg;
		msg = systemId + " the message is not for me";
		// TODO LogDDSInfo(msg);
		return false;
	}

	// prevent history data
	if ((currentTime - msgdata.time) > 10e-5) {
		str = "Old Data {";
		str = str + str_time + "} at {"
			+ to_string(currentTime) + "}";
		string msg;
		msg = systemId + str;
		// TODO LogDDSInfo(msg);
		return false;
	}

	if (tName == ACQUIRE_READY_STATE) {
		MsgData msgdata;
		msgdata.content = to_string(currentTime);
		msgdata.from = nodeName;
		msgdata.systemId = systemId;
		msgdata.time = currentTime;
		msgdata.topicName = NODE_READY;
		if (!inst->write(NODE_READY,msgdata))
		{
			string msg;
			msg = systemId + " data send fail at " + to_string(currentTime);
			// TODO LogDDSErr(msg);
			return false;
		}

		//p_DDSapi->publish(NODE_READY, "me");
	}
	else if (tName == INITIAL_FEDERATE) {
		currentTime = msgdata.time;
		
		stringstream ss;
		ss << msgdata.content;
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

			char* datac = p_JSONapi->ConvertJson2TypeData(key, value);

			(*p_setToTool)(currentTime, cstr, (void*)datac);
			delete[] cstr;
			delete[] cstr2;
			iter++;
		}
		currentTime = msgdata.time;
		(*p_setFinish)(currentTime);
	}
	else if (tName == SIMULATION_END) {
		// TODO
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