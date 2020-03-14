
#include "stdafx.h"
#include "Interface.h"

bool wsServe(dds::core::cond::WaitSet waitSet, string systemId) {

#ifdef STDOUTTEST
	cout << "[info] <" << systemId << "> "
		<< "dds detached thread starts well" << endl;
#endif
	string msg;
	msg = systemId + " dds detached thread starts";
	// TODO LogDDSInfo(msg);
	while (1) {
		try {
			waitSet.dispatch();
		}
		catch (const dds::core::TimeoutError e) {

#ifdef STDOUTTEST
			cout << "[error] <" << systemId << "> "
				"dds thread:" << e.what() << endl;
#endif
			msg = systemId + " dds thread : " + e.what();
			// TODO LogDDSErr(msg);
			return false;
		}
	}

#ifdef STDOUTTEST
	cout << "[info] <" << systemId << "> "
		<< "dds detached thread ends" << endl;
#endif
	msg = systemId + " dds detached thread ends";
	// TODO LogDDSInfo(msg);
	return true;
}

Interface::Interface() {
	// TODO 多实例配置
	systemRunId = org::opensplice::domain::default_id();

	p_XMLapi = new CSScenarioXML();
	p_JSONapi = new JSONapi(p_XMLapi);
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

		// 2.Parameters
		currentTime = 0.0;
		this->p_initTool = p_initTool;
		this->p_setToTool = p_setToTool;
		this->p_setFinish = p_setFinish;
		this->p_endTool = p_endTool;

		// 3.DDS
		p_DDSapi = new DDSapi(this);
		if (!p_DDSapi->startServerDDS()) {

#ifdef STDOUTTEST
			cout << "[error] <" << systemId << "> "
				<< "starts dds fail" << endl;
#endif
			msg = systemId + " starts dds fail";
			// TODO LogDDSErr(msg);
			return "";
		}

		thread th(wsServe, p_DDSapi->waitSet, systemId);
		th.detach();

#ifdef STDOUTTEST
		cout << "-----CONGRATULATIONS, ALMOST DONE!-----" << endl;
#endif
		// TODO 处理
		Sleep(1000);
		if (p_DDSapi->publish(NODE_READY, "me")) {
			return systemId;
		}
		else {
			return "";
		}
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
	if (!p_DDSapi->publish(topic_name.c_str(), data)) {
		string msg;
		msg = systemId + " data send fail at " + to_string(currentTime);
		// TODO LogDDSErr(msg);
		return false;
	}
	return true;
}

bool Interface::advance() {
	if (!p_DDSapi->publish(ADVANCE_REQUEST, to_string(currentTime))) {
		string msg;
		msg = systemId + " advance send fail at " + to_string(currentTime);
		// TODO LogDDSErr(msg);
		return false;
	}
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

bool Interface::process(Msg messageIn) {

	string str;
	string str_time = to_string(messageIn.time());
	string tName = messageIn.topicName();

#ifdef STDOUTTEST
	str = "RECEIVE <";
	str = str + tName + "> FROM <" + messageIn.from()
		+ "> AT <" + str_time + ">";
	cout << str << endl;
#endif

	string sName = messageIn.systemId();
	if (sName != systemId) {
		string msg;
		msg = systemId + " the message is not for me";
		// TODO LogDDSInfo(msg);
		return false;
	}

	// prevent history data
	if ((currentTime - messageIn.time()) > 10e-5) {
		str = "Old Data {";
		str = str + str_time + "} at {"
			+ to_string(currentTime) + "}";
		string msg;
		msg = systemId + str;
		// TODO LogDDSInfo(msg);
		return false;
	}

	if (tName == ACQUIRE_READY_STATE) {
		p_DDSapi->publish(NODE_READY, "me");
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

			char* datac = p_JSONapi->ConvertJson2TypeData(key, value);

			(*p_setToTool)(currentTime, cstr, (void*)datac);
			delete[] cstr;
			delete[] cstr2;
			iter++;
		}
		currentTime = messageIn.time();
		(*p_setFinish)(currentTime);
	}
	else if (tName == SIMULATION_END) {
		// TODO
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
