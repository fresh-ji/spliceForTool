
#ifndef _INTERFACE_
#define _INTERFACE_

#include <thread>
#include <iostream>
#include <fstream>
#include <random>
#include <stdlib.h>
#include <cassert>

#include "WaitSetData_DCPS.hpp"

#include "csscenario_xml.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

///#include "cssimlog.h"

using namespace rapidjson;

typedef void(*initTool)(double, double);
typedef void(*setToTool)(double, char*, void*);
typedef void(*setFinish)(double);
typedef void(*endTool)();

#define ACQUIRE_READY_STATE "acquire_ready_state"
#define NODE_READY "node_ready"
#define INITIAL_FEDERATE "initial_federate"
#define ADVANCE_REQUEST "advance_request"
#define ADVANCE_GRANT "advance_grant"
#define SIMULATION_RUN "simulation_run"
#define SIMULATION_END "simulation_end"

#define ENGINENAME "SIMUengine777"

#define STDOUTTEST

using namespace std;
using namespace WaitSetData;

class Interface {

public:
	Interface();
	// real interfaces
	string start(string configName,
		initTool p_initTool, setToTool p_setToTool,
		setFinish p_setFinish, endTool p_endTool);
	bool setValue(string name, void* data);
	bool advance();
	bool end();

	bool process(Msg);

private:
	// node management
	string systemId;
	int systemRunId;
	string nodeName;

	// ���������ļ�
	bool parseConfig();

	//ת���ṹ��������json
	string ConvertTypeData2Json(string name, void* data);

	//ת��json���ṹ������
	char* ConvertJson2TypeData(string name, string data);

	// time management
	double currentTime;
	double step;

	// ���ķ���
	vector<string> pubNames;
	vector<string> subNames;

	// DDS���
	map<string, dds::pub::DataWriter<Msg>> writers;
	map<string, dds::sub::DataReader<Msg>> readers;
	dds::core::cond::WaitSet waitSet;
	bool startServerDDS();
	bool publish(string, string);

	// data management
	map<string, string> dataMap;
	map<string, string> backupDataMap;

	// callback
	initTool p_initTool;
	setToTool p_setToTool;
	setFinish p_setFinish;
	endTool p_endTool;

	// �����ļ�����
	CSScenarioXML xml_parser_;
};

#endif
