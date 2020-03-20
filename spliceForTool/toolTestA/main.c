
#include <tchar.h>
#include <Windows.h>
#include <stdio.h>

/*
* -------INSA-------
*/

struct UDPosition {
	double longitude;
	double latitude;
	double altitude;
	double x;
	double y;
	double z;
};

struct UDPosture {
	int vx;
	int vy;
	int vz;
	int phi;
	int psi;
	int gamma;
};

void initTool(double, double);
void setToTool(double, char*, void*);
void setFinish(double);
void endTool();

int endFlag = 0;
char* token;
double topic_001_data = 0;

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
}

void setFinish(double time) {
	topic_001_data += 2.7;
	setFun(token, "topic_001", (void*)&topic_001_data);

	printf("i did something and go forward to %f\n", time);
	advanceFun(token);
}

void endTool() {
	endFun(token);
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

	token = startFun("Inka6XNh_insA.xml",
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
