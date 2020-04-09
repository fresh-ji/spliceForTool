
#include "stdafx.h"
#include "spliceForTool.h"

Interface *inst = NULL;

// 1
char* dllStart(char* configName,
	initTool p_initTool, setToTool p_setToTool,
	setFinish p_setFinish, endTool p_endTool) {

	inst = Interface::Instance();
	string ret = inst->start(configName,
		p_initTool, p_setToTool, p_setFinish, p_endTool);

	char* data = new char[strlen(ret.c_str()) + 1];
	strcpy(data, ret.c_str());

	return data;
}

// 2
int dllSetValue(char* name, void* data) {
	if (inst) {
		return (inst->setValue(name, data) ? 1 : 0);
	}
	return 0;
}

// 3
int dllAdvance() {
	if (inst) {
		return (inst->advance() ? 1 : 0);
	}
	return 0;
}

// 4
int dllEnd() {
	if (inst) {
		return (inst->end() ? 1 : 0);
	}
	return 0;
}
