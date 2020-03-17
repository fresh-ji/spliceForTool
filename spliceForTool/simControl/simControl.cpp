// simControl.cpp : 定义 DLL 应用程序的导出函数。
//

//#include "stdafx.h"
#include "simControl.h"

// 1
int dllRun(char* configName) {
	return (SimControlInterface::getInstance().simRun(configName) ? 1 : 0);
	//return 0;
}

// 2
int dllPause() {
	return (SimControlInterface::getInstance().simPause() ? 1 : 0);
	//return 0;
}

// 3
int dllContinue() {
	return (SimControlInterface::getInstance().simContinue() ? 1 : 0);
	//return 0;
}

// 4
int dllEnd() {
	return (SimControlInterface::getInstance().simEnd() ? 1 : 0);
	//return 0;
}


