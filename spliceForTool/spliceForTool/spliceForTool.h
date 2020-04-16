
// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� SPLICEFORTOOL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// SPLICEFORTOOL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�

#include "Interface.h"
#include "spliceForTool_export.h"

/**
* @brief       dllStart                         dll����
* @param       [in]               configName    �����ļ���
* @param       [in]               p_initTool    ��ʼ������
* @param       [in]               p_setToTool   �򹤾�������
* @param       [in]               p_setFinish   �������ƽ�
* @param       [in]               p_endTool     ��������
*/
extern "C" SPLICEFORTOOL_EXPORT char* dllStart(char* configName,
	initTool p_initTool, setToTool p_setToTool,
	setFinish p_setFinish, endTool p_endTool);

/**
* @brief       dllSetValue                      ��dll������
* @param       [in]               name          ������
* @param       [in]               data          ����ֵ
*/
extern "C" SPLICEFORTOOL_EXPORT int dllSetValue(char* name, void* data);

/**
* @brief       dllAdvance                       ��dll�����ƽ�
*/
extern "C" SPLICEFORTOOL_EXPORT int dllAdvance();

/**
* @brief       dllEnd						    ��dll�������
*/
extern "C" SPLICEFORTOOL_EXPORT int dllEnd();

extern "C" SPLICEFORTOOL_EXPORT int dllStopDDS();