
#include <tchar.h>
#include <Windows.h>
#include <stdio.h>

//INSB

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
} UDPosture;

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
	if (strcmp(name, "topic_003") == 0) {
		UDPosture* pos = (UDPosture*)data;
		printf("info:\n");
		printf("vx : %d\n", pos->vx);
		printf("vy : %d\n", pos->vy);
		printf("vz : %d\n", pos->vz);
		printf("phi : %d\n", pos->phi);
		printf("psi : %d\n", pos->psi);
		printf("gamma : %d\n", pos->gamma);
	}
}

void setFinish(double time) {
	pos.longitude = pos.longitude + 1;
	pos.latitude = pos.latitude + 1;
	pos.altitude = pos.altitude + 1;
	pos.x = pos.x + 1;
	pos.y = pos.y + 1;
	pos.z = pos.z + 1;

	setFun("topic_002", (void*)&pos);
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
	//HMODULE hInstC = LoadLibrary(_T("spliceForTool"));
	if (hInstC == NULL) {
		err = GetLastError();
		printf("load dll fail %d", err);
		getchar();
	}

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

	token = startFun("Gr342ttL_insB.xml",
		initTool, setToTool, setFinish, endTool);

	if ("" == token) {
		printf("start wrong\n");
	}
	else {
		printf("everything fine\n");
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
