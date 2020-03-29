
#ifndef _INTERFACE_
#define _INTERFACE_

#include <string>
#include <iostream>
#include <sstream>

#include "JSONapi.h"
#include "dds_service.h"

#include "cssimlog.h"
#include "cssimlog_export.h"

class Interface {

public:
	static Interface* Instance();

	// real interfaces
	string start(string configName,
		initTool p_initTool, setToTool p_setToTool,
		setFinish p_setFinish, endTool p_endTool);
	bool setValue(string name, void* data);
	bool advance();
	bool end();

	~Interface();
	Interface(const Interface&) = delete;
	Interface& operator=(const Interface&) = delete;

private:
	Interface();
	bool parseConfig();
	bool process(const MsgData&  msgdata);
	bool publish(string, string);

	string systemId;
	// int systemRunId;
	string nodeName;
	vector<string> pubNames;
	vector<string> subNames;

	// time management
	double currentTime;
	double step;

private:
	// data management
	map<string, string> dataMap;
	map<string, string> backupDataMap;

	// callback
	initTool p_initTool;
	setToTool p_setToTool;
	setFinish p_setFinish;
	endTool p_endTool;

public:
	JSONapi *p_JSONapi;
	CSScenarioXML *p_XMLapi;
	CSDDSService *p_ddsInst;

public:
	void matInit();
	void matData();
	void matFinish();
};

#endif
