
// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 SPLICEFORTOOL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// SPLICEFORTOOL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。

#ifdef SPLICEFORTOOL_EXPORTS
#define SPLICEFORTOOL_API __declspec(dllexport)
#else
#define SPLICEFORTOOL_API __declspec(dllimport)
#endif

#include "Interface.h"

/**
* @brief       dllStart                         dll启动
* @param       [in]               p_initTool    初始化工具
* @param       [in]               p_setToTool   向工具推数据
* @param       [in]               p_setFinish   允许工具推进
* @param       [in]               p_endTool     结束工具
*/
extern "C" SPLICEFORTOOL_API int dllStart(initTool p_initTool,
	setToTool p_setToTool, setFinish p_setFinish, endTool p_endTool);

/**
* @brief       dllSetValue                      向dll推数据
* @param       [in]               name          数据名
* @param       [in]               data          数据值
*/
extern "C" SPLICEFORTOOL_API int dllSetValue(char* name, void* data);

/**
* @brief       dllAdvance                       向dll请求推进
*/
extern "C" SPLICEFORTOOL_API int dllAdvance();

/**
* @brief       dllAdvance                       向dll请求结束
*/
extern "C" SPLICEFORTOOL_API int dllEnd();
