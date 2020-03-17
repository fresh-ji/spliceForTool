// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 SPLICEFORTOOL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// SPLICEFORTOOL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。

#ifdef SIMCONTROL_EXPORTS
#define SIMCONTROL_API __declspec(dllexport)
#else
#define SIMCONTROL_API __declspec(dllimport)
#endif

#include "simControlInterface.h"

/**
* @brief       dllRun                           dll开始
*/
extern "C" SIMCONTROL_API int dllRun(char* configName);

/**
* @brief       dllPause                         dll暂停
*/
extern "C" SIMCONTROL_API int dllPause();

/**
* @brief       dllContinue                      dll继续
*/
extern "C" SIMCONTROL_API int dllContinue();

/**
* @brief       dllEnd                           dll停止
*/
extern "C" SIMCONTROL_API int dllEnd();
