
#include <tchar.h>
#include <Windows.h>
#include <stdio.h>
#include <iostream>

/**** Ԥ����� ****/
// �ص�����
void initTool(double, double);
void setToTool(double, char*, void*);
void setFinish(double);
void endTool();
//�ӿڶ���
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
/**** Ԥ����� ****/

/**** TODO ���ⶨ�����ݽṹ ****/

struct data {
	double d;
	int i;
	std::string s;
} data_;

/**** TODO ���ⶨ�����ݽṹ ****/

/**** TODO ���ⶨ������ ****/

/**** TODO ���ⶨ������ ****/

void initTool(double startTime, double step) {
	/**** TODO ������з���ǰ�ĳ�ʼ�� ****/

	/**** TODO ������з���ǰ�ĳ�ʼ�� ****/
	/**** Ԥ����� ****/
	printf("i should start at %f and step is %f\n", startTime, step);
	advanceFun();
	/**** Ԥ����� ****/
}

void setToTool(double time, char* name, void* data) {
	/**** TODO ����������������� ****/

	/**** TODO ����������������� ****/
}

void setFinish(double time) {
	/**** TODO ������в����ƽ������ݷ��� ****/

	data_.d = 3.0;
	data_.i = 9;
	data_.s = "hello";
	setFun("io", (void*)&data_);

	std::string ss = "world";

	setFun("ss", (void*)&ss);

	/**** TODO ������в����ƽ������ݷ��� ****/
	/**** Ԥ����� ****/
	printf("i did something and go forward to %f\n", time);
	advanceFun();
	/**** Ԥ����� ****/
}

void endTool() {
	/**** Ԥ����� ****/
	printf("i am over, please close me soon\n");
	endFlag = 1;
	/**** Ԥ����� ****/
}

int main(int argc, char *argv[]) {

	/**** Ԥ����� ****/
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

	/**** TODO ����Ҫд���ص������ļ���·�� ****/
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
	/**** Ԥ����� ****/
	system("pause");
	return 1;
}
