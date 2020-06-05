
#include <tchar.h>
#include <Windows.h>
#include <stdio.h>

/**** 预留勿改 ****/
// 回调定义
void initTool(double, double);
void setToTool(double, char*, void*);
void setFinish(double);
void endTool();
//接口定义
typedef char*(*FunDLL1)(char*,
	void(*initTool)(double, double),
	void(*setToTool)(double, char*, void*),
	void(*setFinish)(double),
	void(*endTool)());
FunDLL1 startFun;
typedef int(*FunDLL2)(char*, void*);
FunDLL2 setFun;
typedef int(*FunDLL3)();
FunDLL3 advanceFun;
typedef int(*FunDLL4)();
FunDLL4 endFun;
int endFlag = 0;
/**** 预留勿改 ****/

/**** TODO 在这定义数据结构 ****/

struct data {
	double d;
	int i;
} data_;

/**** TODO 在这定义数据结构 ****/

/**** TODO 在这定义数据 ****/

/**** TODO 在这定义数据 ****/

void initTool(double startTime, double step) {
	/**** TODO 在这进行仿真前的初始化 ****/

	/**** TODO 在这进行仿真前的初始化 ****/
	/**** 预留勿改 ****/
	printf("i should start at %f and step is %f\n", startTime, step);
	advanceFun();
	/**** 预留勿改 ****/
}

void setToTool(double time, char* name, void* data) {
	/**** TODO 在这个方法里收数据 ****/

	/**** TODO 在这个方法里收数据 ****/
}

void setFinish(double time) {
	/**** TODO 在这进行步长推进和数据发送 ****/

	data_.d = 3.0;
	data_.i = 9;
	setFun("io", (void*)&data_);

	/**** TODO 在这进行步长推进和数据发送 ****/
	/**** 预留勿改 ****/
	printf("i did something and go forward to %f\n", time);
	advanceFun();
	/**** 预留勿改 ****/
}

void endTool() {
	/**** 预留勿改 ****/
	printf("i am over, please close me soon\n");
	endFlag = 1;
	/**** 预留勿改 ****/
}

int main(int argc, char *argv[]) {

	/**** 预留勿改 ****/
	DWORD err = 0;
	HMODULE hInstC = LoadLibraryEx(_T("spliceForTool"), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if (hInstC == NULL) {
		err = GetLastError();
		printf("load dll fail %d", err);
		system("pause");
	}

	startFun = (FunDLL1)GetProcAddress(hInstC, "dllStart");
	setFun = (FunDLL2)GetProcAddress(hInstC, "dllSetValue");
	advanceFun = (FunDLL3)GetProcAddress(hInstC, "dllAdvance");
	endFun = (FunDLL4)GetProcAddress(hInstC, "dllEnd");

	/**** TODO 这里要写下载的配置文件的路径 ****/
	char* token = startFun("T3pCRMA6_inst.xml",
		initTool, setToTool, setFinish, endTool);

	if (strcmp(token, "") == 0) {
		printf("start wrong\n");
	}
	else {
		printf("start fine\n");
		while (1) {
			Sleep(30);
			if (endFlag == 1) {
				// TODO
				//FreeLibrary(hInstC);
				break;
			}
		}
	}
	/**** 预留勿改 ****/
	system("pause");
	return 1;
}
