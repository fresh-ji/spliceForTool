
#include <tchar.h>
#include <Windows.h>
#include <stdio.h>

// INSA

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

void initTool(double, double);
void setToTool(double, char*, void*);
void setFinish(double);
void endTool();

int endFlag = 0;
char* token;

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

static double topic_data;

void initTool(double startTime, double step) {
	topic_data = 0;
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
	else if (strcmp(name, "topic_003") == 0) {
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
	topic_data += 1;
	setFun("topic_001", (void*)&topic_data);
	printf("i did something and go forward to %f\n", time);
	advanceFun();
}

void endTool() {
	printf("i am over\n");
}

int main(int argc, char *argv[]) {

	DWORD err = 0;

	HMODULE hInstC = LoadLibraryEx(_T("spliceForTool"), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if (hInstC == NULL) {
		err = GetLastError();
		printf("load dll fail %d", err);
		getchar();
	}

	startFun = (FunDLL1)GetProcAddress(hInstC, "dllStart");
	setFun = (FunDLL2)GetProcAddress(hInstC, "dllSetValue");
	advanceFun = (FunDLL3)GetProcAddress(hInstC, "dllAdvance");
	endFun = (FunDLL4)GetProcAddress(hInstC, "dllEnd");

	token = startFun("Gr342ttL_insA.xml",
		initTool, setToTool, setFinish, endTool);

	if (strcmp(token, "") == 0) {
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
