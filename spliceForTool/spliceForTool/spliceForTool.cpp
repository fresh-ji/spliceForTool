
#include "stdafx.h"
#include "spliceForTool.h"

map<string, Interface*> instanceMap;

// 1
char* dllStart(char* configName,
	initTool p_initTool, setToTool p_setToTool,
	setFinish p_setFinish, endTool p_endTool) {

	Interface* i = new Interface();

	string ret = i->start(configName,
		p_initTool, p_setToTool, p_setFinish, p_endTool);

	if ("" != ret) {
		if (1 == instanceMap.count(ret)) {
			instanceMap.erase(ret);
			//TODO log
		}
		instanceMap.insert(make_pair(ret, i));
		//TODO log
	}
	else {
		//TODO log
	}

	char* data = new char[strlen(ret.c_str()) + 1];
	strcpy(data, ret.c_str());

	return data;
}

// 2
int dllSetValue(char* token, char* name, void* data) {
	if (0 == instanceMap.count(token)) {
		//TODO log
		return 0;
	}
	return (instanceMap[token]->setValue(name, data) ? 1 : 0);
}

// 3
int dllAdvance(char* token) {
	if (0 == instanceMap.count(token)) {
		//TODO log
		return 0;
	}
	return (instanceMap[token]->advance() ? 1 : 0);
}

// 4
int dllEnd(char* token) {
	if (0 == instanceMap.count(token)) {
		//TODO log
		return 0;
	}
	return (instanceMap[token]->end() ? 1 : 0);
}
