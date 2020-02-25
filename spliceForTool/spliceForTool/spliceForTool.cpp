// spliceForTool.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "spliceForTool.h"


// 这是导出变量的一个示例
SPLICEFORTOOL_API int nspliceForTool=0;

// 这是导出函数的一个示例。
SPLICEFORTOOL_API int fnspliceForTool(void)
{
	return 42;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 spliceForTool.h
CspliceForTool::CspliceForTool()
{
	return;
}
