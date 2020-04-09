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

	//SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);

	//char path[MAX_PATH];
	//if (GetModuleFileName(NULL, path, MAX_PATH)>0)
	//{
	//	(*strrchr(path, '\\')) = '\0';//丢掉文件名，得到路径   
	//}

	//int nLength = MultiByteToWideChar(CP_ACP, 0, path, -1, NULL, NULL);
	//std::wstring wszStr_path;
	//wszStr_path.resize(nLength);
	//LPWSTR lpwszStr = new wchar_t[nLength];
	//MultiByteToWideChar(CP_ACP, 0, path, -1, lpwszStr, nLength);
	//wszStr_path = lpwszStr;

	////auto current_path = fs::current_path();
	//auto str = wszStr_path + std::wstring(
	//	L"/external/OpenSplice/x64/bin");
	//AddDllDirectory(str.c_str());

	//std::string file_path = path;
	//file_path.append("/external/OpenSplice/x64/etc/config/ospl.xml");
	//std::string env("file://");
	//env.append(file_path);
	//errno_t er = _putenv_s("OSPL_URI", env.c_str());

	DWORD err = 0;

	//HMODULE hInstC = LoadLibraryEx(_T("simControl"), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);//simControl //spliceForTool
	HMODULE hInstC = LoadLibrary(_T("simControl"));
	if (hInstC == NULL) {
		err = GetLastError();
		printf("load dll fail %d", err);
		getchar();
	}

	simRunFun = (FunDLL1)GetProcAddress(hInstC, "dllRun");
	simPauseFun = (FunDLL2)GetProcAddress(hInstC, "dllPause");
	simContinueFun = (FunDLL3)GetProcAddress(hInstC, "dllContinue");
	simEndFun = (FunDLL4)GetProcAddress(hInstC, "dllEnd");

	char file_name[64] = "Gr342ttL.xml";
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






