// spliceForTool.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "spliceForTool.h"


// ���ǵ���������һ��ʾ��
SPLICEFORTOOL_API int nspliceForTool=0;

// ���ǵ���������һ��ʾ����
SPLICEFORTOOL_API int fnspliceForTool(void)
{
	return 42;
}

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� spliceForTool.h
CspliceForTool::CspliceForTool()
{
	return;
}
