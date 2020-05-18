
#include <tchar.h>
#include <Windows.h>
#include <stdio.h>

// INSC

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

static UDPosture posture;

void initTool(double, double);
void setToTool(double, char*, void*);
void setFinish(double);
void endTool();

int endFlag = 0;
char* token;
HMODULE hInstC;

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
	posture.vx = 0;
	posture.vy = 0;
	posture.vz = 0;
	posture.psi = 0;
	posture.phi = 0;
	posture.gamma = 0;
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
	else if (strcmp(name, "topic_001") == 0)
	{
		double d = *(double*)data;
		printf("topic_001 : %f\n", d);
	}
}

void setFinish(double time) {

	posture.vx = posture.vx + 100;
	posture.vy = posture.vy + 100;
	posture.vz = posture.vz + 100;
	posture.psi = posture.psi + 100;
	posture.phi = posture.phi + 100;
	posture.gamma = posture.gamma + 100;

	setFun("topic_003", (void*)&posture);

	printf("i did something and go forward to %f\n", time);
	advanceFun();
}

void endTool() {
	printf("i am over, please close me soon\n");
	FreeLibrary(hInstC);
	endFlag = 1;
}

int main(int argc, char *argv[]) {

	DWORD err = 0;

	hInstC = LoadLibraryEx(_T("spliceForTool"), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if (hInstC == NULL) {
		err = GetLastError();
		printf("load dll fail %d", err);
		getchar();
	}

	startFun = (FunDLL1)GetProcAddress(hInstC, "dllStart");
	setFun = (FunDLL2)GetProcAddress(hInstC, "dllSetValue");
	advanceFun = (FunDLL3)GetProcAddress(hInstC, "dllAdvance");
	endFun = (FunDLL4)GetProcAddress(hInstC, "dllEnd");

	token = startFun("Gr342ttL_insC.xml",
		initTool, setToTool, setFinish, endTool);

	if ("" == token) {
		printf("start wrong\n");
	}
	else{
		printf("everything fine\n");
		while (1) {
			Sleep(30);
			if (endFlag == 1) {
				break;
			}
		}
	}

	getchar();
	return 1;
}
