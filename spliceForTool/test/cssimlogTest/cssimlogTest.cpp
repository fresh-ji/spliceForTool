// cssimlogTest.cpp : 定义控制台应用程序的入口点。
//

//#include "stdafx.h"
#include "cssimlog.h"
#include "cssimlog_export.h"

int main(int argc, char *argv[])
{
	std::string pt("initialize_log.txt");
	CSSimLog::Instance()->CreateLog(pt);
	LogDDSInfo("init log success");

	LogSETrace("trace");
	LogSEDebug("debug");
	LogSEInfo("info");
	LogSEWarn("warn");
	LogSEErr("error");
	LogSECri("cri");

	LogDDSTrace("trace");
	LogDDSDebug("debug");
	LogDDSInfo("info");
	LogDDSWarn("warn");
	LogDDSErr("error");
	LogDDSCri("cri");

	CSSimLog::Instance()->CloseLog();
	getchar();
	return 0;
}

