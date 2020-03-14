
#ifndef _INTERFACE_
#define _INTERFACE_

#include <thread>

//#include <fstream>
//#include <random>
//#include <stdlib.h>
//#include <cassert>

#include "definition.h"
#include "DDSapi.h"
#include "JSONapi.h"
#include "XMLapi.h"

class DDSapi;

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

public:
	string systemId;
	int systemRunId;
	string nodeName;
	vector<string> pubNames;
	vector<string> subNames;

	// time management
	double currentTime;
	double step;

private:
	// Ω‚Œˆ≈‰÷√Œƒº˛
	bool parseConfig();

	// data management
	map<string, string> dataMap;
	map<string, string> backupDataMap;

	// callback
	initTool p_initTool;
	setToTool p_setToTool;
	setFinish p_setFinish;
	endTool p_endTool;

public:
	DDSapi *p_DDSapi;
	JSONapi *p_JSONapi;
	CSScenarioXML *p_XMLapi;
};

#endif
