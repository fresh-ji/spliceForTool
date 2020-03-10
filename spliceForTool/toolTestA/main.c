
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

typedef struct _UDPosture {
	int vx;
	int	vy;
	int	vz;
	int	phi;
	int	psi;
	int	gamma;
}UDPosture;

static double topic_data = 0;

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
		printf("longitude : %f\n", pos->longitude);
		printf("latitude : %f\n", pos->latitude);
		printf("altitude : %f\n", pos->altitude);
		printf("x : %f\n", pos->x);
		printf("y : %f\n", pos->y);
		printf("z : %f\n", pos->z);
	}
	else if (strcmp(name, "topic_003") == 0)
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

	//setFinish(time);
}

void setFinish(double time) {
	topic_data += 1;
	setFun("topic_001", (void*)&topic_data);
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

	startFun("E:\\2y\\spliceForTool\\ZtOE0Jfu_insA.xml",
		initTool, setToTool, setFinish, endTool);

	/*while (1) {
		Sleep(30);
		if (endFlag == 1) {
			FreeLibrary(hInstC);
			break;
		}
	}*/

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
	/*while (1)
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
	}*/

	getchar();
	return 1;
}






