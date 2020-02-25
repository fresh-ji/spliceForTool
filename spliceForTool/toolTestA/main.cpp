// toolTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

//#include "pch.h"
#include <iostream>
#include <tchar.h>
#include <Windows.h>
#include <map>

#include <experimental/filesystem>
#include <filesystem>

using namespace std;
namespace  fs = std::experimental::filesystem::v1;

typedef struct _UDPosition {
	double longitude;
	double latitude;
	double altitude;
	double x;
	double y;
	double z;
}UDPosition;

void initTool(double, double);
void setToTool(double, char*, void*);
void setFinish(double);
void endTool();

// 1
typedef int(*FunDLL1)(void(*initTool)(double, double),
	void(*setToTool)(double, char*, void*),
	void(*setFinish)(double),
	void(*endTool)());
FunDLL1 startFun;
// 2
typedef int(*FunDLL2)(char*, void*);
FunDLL2 setFun;
// 3
typedef int(*FunDLL3)();
FunDLL3 advanceFun;
// 4
typedef int(*FunDLL4)();
FunDLL4 endFun;

void initTool(double startTime, double step) {
	cout << "i should start at " << startTime
		<< " and my step is " << step << endl;
}

void setToTool(double time, char* name, void* data) {
	cout << "i received data at " << time << " name" << name << endl;
	if (strcmp(name,"topic_002") == 0){
		UDPosition* pos = (UDPosition*)data;
		cout << "longitude:" << pos->longitude << endl;
		cout << "latitude:" << pos->latitude << endl;
		cout << "altitude:" << pos->altitude << endl;
		cout << "x:" << pos->x << endl;
		cout << "y:" << pos->y << endl;
		cout << "z:" << pos->z << endl;
	}
}

void setFinish(double time) {
	cout << "i did something and go forward to " << time << endl;
	advanceFun();
}

void endTool() {
	endFun();
	cout << "i am over" << endl;
}

int main(int argc, char *argv[]) {

	DWORD err = 0;

	//HMODULE hInstC = LoadLibraryEx(_T("spliceForTool"), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	HMODULE hInstC = LoadLibraryEx(_T("spliceForTool"), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

	SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
	auto current_path = fs::current_path();
	auto str = current_path.generic_wstring() + std::wstring(
		L"/external/OpenSplice/bin");
	AddDllDirectory(str.c_str());


	std::string file_path = current_path.generic_string();
	file_path.append("/external/OpenSplice/etc/config/ospl.xml");
	std::string env("file://");
	env.append(file_path);
	errno_t er = _putenv_s("OSPL_URI", env.c_str());
	if (er) {
		cout << "set dds env error, error code:" << std::to_string(er);
	}

	//HMODULE hInstC = LoadLibraryA("spliceForTool.dll");
	
	if (hInstC == NULL) {
		cout << "load dll fail" << endl;
		err = GetLastError();
		cout << err << endl;
		getchar();
	}

	startFun = (FunDLL1)GetProcAddress(hInstC, "dllStart");
	setFun = (FunDLL2)GetProcAddress(hInstC, "dllSetValue");
	advanceFun = (FunDLL3)GetProcAddress(hInstC, "dllAdvance");
	endFun = (FunDLL4)GetProcAddress(hInstC, "dllEnd");

	startFun(initTool, setToTool, setFinish, endTool);

	UDPosition pos;
	pos.longitude = 0.0;
	pos.latitude = 0.0;
	pos.altitude = 0.0;
	pos.x = 0.0;
	pos.y = 0.0;
	pos.z = 0.0;
	char topic_name[10] = "topic_002";
	char topic_name2[20] = "advance_grant";
	int time = 1;
	while (1)
	{
		pos.longitude = pos.longitude + 1;
		pos.latitude = pos.latitude + 1;
		pos.altitude = pos.altitude + 1;
		pos.x = pos.x + 1;
		pos.y = pos.y + 1;
		pos.z = pos.z + 1;

		setFun(topic_name, (void*)&pos);
		if (time % 3 == 0)
		{
			setFun(topic_name2, (void*)NULL);
		}

		time++;
		Sleep(1000);
	}

	getchar();

	return 1;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
