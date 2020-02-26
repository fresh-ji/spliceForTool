
#include <tchar.h>
#include <Windows.h>
#include <stdio.h>

//#include <experimental/filesystem>
//#include <filesystem>

//using namespace std;
//namespace fs = std::experimental::filesystem::v1;

typedef struct _UDPosition {
	double longitude;
	double latitude;
	double altitude;
	double x;
	double y;
	double z;
} UDPosition;

void initTool(double, double);
void setToTool(double, char*, void*);
void setFinish(double);
void endTool();

int endFlag = 0;

// 1
typedef int(*FunDLL1)(char*,
	void(*initTool)(double, double),
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
	printf("i should start at %f and step is %f\n", startTime, step);
	advanceFun();
}

void setToTool(double time, char* name, void* data) {
	printf("i received data at %f for %s\n", time, name);
	if (strcmp(name, "topic_002") == 0) {
		UDPosition* pos = (UDPosition*)data;
		printf("info:\n");
		printf("longitude : %s\n", pos->longitude);
		printf("latitude : %s\n", pos->latitude);
		printf("altitude : %s\n", pos->altitude);
		printf("x : %s\n", pos->x);
		printf("y : %s\n", pos->y);
		printf("z : %s\n", pos->z);
	}
}

void setFinish(double time) {
	printf("i did something and go forward to %f\n", time);
	advanceFun();
}

void endTool() {
	endFun();
	printf("i am over\n");
	endFlag = 1;
}

int main(int argc, char *argv[]) {

	DWORD err = 0;

	HMODULE hInstC = LoadLibraryEx(_T("spliceForTool"), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if (hInstC == NULL) {
		err = GetLastError();
		printf("load dll fail %d", err);
		getchar();
	}

	/*
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
	*/

	startFun = (FunDLL1)GetProcAddress(hInstC, "dllStart");
	setFun = (FunDLL2)GetProcAddress(hInstC, "dllSetValue");
	advanceFun = (FunDLL3)GetProcAddress(hInstC, "dllAdvance");
	endFun = (FunDLL4)GetProcAddress(hInstC, "dllEnd");

	startFun("ZtOE0Jfu_insA.xml",
		initTool, setToTool, setFinish, endTool);

	while (1) {
		Sleep(30);
		if (endFlag == 1) {
			FreeLibrary(hInstC);
			break;
		}
	}

	getchar();
	return 1;
}
