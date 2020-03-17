#include <tchar.h>
#include <Windows.h>

#include <stdio.h>
#include <iostream>
#include <filesystem>

#include <chrono>
#include <thread>

using namespace std;


// 1
typedef int(*FunDLL1)(char*);
FunDLL1 simRunFun;
// 2
typedef int(*FunDLL2)();
FunDLL2 simPauseFun;
// 3
typedef int(*FunDLL3)();
FunDLL3 simContinueFun;
// 4
typedef int(*FunDLL4)();
FunDLL4 simEndFun;

int main(int argc, char *argv[]) {

	DWORD err = 0;

	HMODULE hInstC = LoadLibraryEx(_T("simControl"), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);//simControl //spliceForTool
	if (hInstC == NULL) {
		err = GetLastError();
		printf("load dll fail %d", err);
		getchar();
	}

	simRunFun = (FunDLL1)GetProcAddress(hInstC, "dllRun");
	simPauseFun = (FunDLL2)GetProcAddress(hInstC, "dllPause");
	simContinueFun = (FunDLL3)GetProcAddress(hInstC, "dllContinue");
	simEndFun = (FunDLL4)GetProcAddress(hInstC, "dllEnd");

	char file_name[64] = "E:\\2y\\spliceForTool\\ZtOE0Jfu.xml";
	//simRunFun(file_name);


	char index;
	while (1) {
		cout << "请输入指令:";
		std::cin >> index;
		if (index != '1') {
			std::cout << "指令选择错误,请重新选择" << std::endl;
			continue;
		}

		if (index == '1') {
			simRunFun(file_name);
			std::this_thread::sleep_for(std::chrono::seconds(200000));
			break;
		}
	}

	getchar();
	return 1;
}






