
#include <tchar.h>
#include <Windows.h>
#include <stdio.h>
#include <iostream>

typedef struct _UDPosition {
	double longitude;
	double latitude;
	double altitude;
	double x;
	double y;
	double z;
} UDPosition;

typedef struct _UDPosture {
	int vx;
	int	vy;
	int	vz;
	int	phi;
	int	psi;
	int	gamma;
}UDPosture;

static UDPosition pos;

void initTool(double, double);
void setToTool(double, char*, void*);
void setFinish(double);
void endTool();

int endFlag = 0;
char* token;

// 1
typedef char*(*FunDLL1)(char*,
	void(*initTool)(double, double),
	void(*setToTool)(double, char*, void*),
	void(*setFinish)(double),
	void(*endTool)());
FunDLL1 startFun;
// 2
typedef int(*FunDLL2)(char*, char*, void*);
FunDLL2 setFun;
// 3
typedef int(*FunDLL3)(char*);
FunDLL3 advanceFun;
// 4
typedef int(*FunDLL4)(char*);
FunDLL4 endFun;

void initTool(double startTime, double step) {
	printf("i should start at %f and step is %f\n", startTime, step);
	advanceFun(token);
}

void setToTool(double time, char* name, void* data) {
	printf("i received data at %f for %s\n", time, name);
	/*if (strcmp(name, "topic_002") == 0) {
		UDPosition* pos = (UDPosition*)data;
		printf("info:\n");
		printf("longitude : %s\n", pos->longitude);
		printf("latitude : %s\n", pos->latitude);
		printf("altitude : %s\n", pos->altitude);
		printf("x : %s\n", pos->x);
		printf("y : %s\n", pos->y);
		printf("z : %s\n", pos->z);
	}
	else*/ if (strcmp(name, "topic_003") == 0)
	{
		UDPosture* pos = (UDPosture*)data;
		printf("info:\n");
		printf("vx : %d\n", pos->vx);
		printf("vy : %d\n", pos->vy);
		printf("vz : %d\n", pos->vz);
		printf("phi : %d\n", pos->phi);
		printf("psi : %d\n", pos->psi);
		printf("gamma : %d\n", pos->gamma);
	}
	/*else if (strcmp(name, "topic_001") == 0)
	{
		double d = *(double*)data;
		printf("topic_001 : %f\n", d);
	}*/

}

void setFinish(double time) {
	pos.longitude = pos.longitude + 1;
	pos.latitude = pos.latitude + 1;
	pos.altitude = pos.altitude + 1;
	pos.x = pos.x + 1;
	pos.y = pos.y + 1;
	pos.z = pos.z + 1;

	setFun(token,"topic_002", (void*)&pos);
	printf("i did something and go forward to %f\n", time);
	advanceFun(token);
}

void endTool() {
	endFun(token);
	printf("i am over\n");
	endFlag = 1;
}

int main(int argc, char *argv[]) {

	SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);

	char path[MAX_PATH];
	if (GetModuleFileName(NULL, path, MAX_PATH)>0)
	{
		(*strrchr(path, '\\')) = '\0';//丢掉文件名，得到路径   
	}

	int nLength = MultiByteToWideChar(CP_ACP, 0, path, -1, NULL, NULL);
	std::wstring wszStr_path;
	wszStr_path.resize(nLength);
	LPWSTR lpwszStr = new wchar_t[nLength];
	MultiByteToWideChar(CP_ACP, 0, path, -1, lpwszStr, nLength);
	wszStr_path = lpwszStr;

	//auto current_path = fs::current_path();
	auto str = wszStr_path + std::wstring(
		L"/external/OpenSplice/x64/bin");
	AddDllDirectory(str.c_str());

	std::string file_path = path;
	file_path.append("/external/OpenSplice/x64/etc/config/ospl.xml");
	std::string env("file://");
	env.append(file_path);
	errno_t er = _putenv_s("OSPL_URI", env.c_str());

	DWORD err = 0;

	//HMODULE hInstC = LoadLibraryEx(_T("spliceForTool"), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	HMODULE hInstC = LoadLibrary(_T("spliceForTool")); if (hInstC == NULL) {
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

	pos.longitude = 0.0;
	pos.latitude = 0.0;
	pos.altitude = 0.0;
	pos.x = 0.0;
	pos.y = 0.0;
	pos.z = 0.0;
	char topic_name[10] = "topic_002";
	char topic_name2[20] = "advance_grant";
	int time = 1;

	token = startFun("ZtOE0Jfu_insB.xml",
		initTool, setToTool, setFinish, endTool);

	if ("" == token) {
		printf("start wrong\n");
	}
	else{
		while (1) {
			Sleep(30);
			if (endFlag == 1) {
				FreeLibrary(hInstC);
				break;
			}
		}
	}

	getchar();
	return 1;
}
