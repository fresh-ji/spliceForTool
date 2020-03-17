
#ifndef _INTERFACE_
#define _INTERFACE_

#include <windows.h>
#include <thread>
#include <iostream>
#include <random>
#include <mutex>

#include "WaitSetData_DCPS.hpp"

#include "sim_control_csscenario_xml.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "cssimlog.h"
#include "cssimlog_export.h"

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

using namespace std;
using namespace WaitSetData;

class SimControlInterface {

private:
	SimControlInterface();
	~SimControlInterface() {}

public:
	static SimControlInterface& getInstance() {
		static SimControlInterface instance;
		return instance;
	}


	bool simRun(string configName);
	bool simPause();
	bool simContinue();
	bool simEnd();

	bool process(Msg);

private:
	// node management
	string systemId;
	int systemRunId;
	string nodeName;

	// ���������ļ�
	//bool parseConfig();

	//ת���ṹ��������json
	string ConvertTypeData2Json(string name, void* data);

	//ת��json���ṹ������
	char* ConvertJson2TypeData(string name, string data);

	//�����ڵ�׼��״̬
	bool CheckAllNodeReadyState();

	//�����ڵ㲽������״̬
	bool CheckAllNodeAdvanceRequestState();
	//���ø��ڵ㲽������״̬
	bool ResetAllNodeAdvanceRequestState();

	// time management
	double currentTime;
	double step;

	// ���ķ���
	vector<string> pubNames;
	vector<string> subNames;

	//
	std::map <std::string, bool> ready_state_;
	std::map <std::string, bool> advance_request_state_;

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

	//mutable std::mutex mx_;
	SimControlCSScenarioXML xml_parser_;
	bool start_flag_ = false;
	bool pause_flag_ = false;
	bool first_acquire_flag_ = true;

	long tick_count_ = 0;

	std::chrono::steady_clock::time_point start_time_;
	std::chrono::steady_clock::time_point current_time_;
	std::chrono::steady_clock::time_point pre_time_;

	std::mutex mx_;
};

#endif
