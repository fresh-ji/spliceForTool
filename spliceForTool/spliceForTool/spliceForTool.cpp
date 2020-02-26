
#include "stdafx.h"
#include "spliceForTool.h"

// 1
int dllStart(char* configName,
	initTool p_initTool, setToTool p_setToTool,
	setFinish p_setFinish, endTool p_endTool) {
	return (Interface::getInstance().start(configName,
		p_initTool, p_setToTool,
		p_setFinish, p_endTool) ? 1 : 0);
}

// 2
int dllSetValue(char* name, void* data) {
	return (Interface::getInstance().setValue(name, data) ? 1 : 0);
}

// 3
int dllAdvance() {
	return (Interface::getInstance().advance() ? 1 : 0);
}

// 4
int dllEnd() {
	return (Interface::getInstance().end() ? 1 : 0);
}
