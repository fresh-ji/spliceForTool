// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� SPLICEFORTOOL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// SPLICEFORTOOL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�

//#ifdef SIMCONTROL_EXPORTS
//#define SIMCONTROL_API __declspec(dllexport)
//#else
//#define SIMCONTROL_API __declspec(dllimport)
//#endif

#include "simControlInterface.h"
#include "simControl_export.h"

/**
* @brief       dllRun                           dll��ʼ
*/
extern "C" SIMCONTROL_EXPORT int dllRun(char* configName);

/**
* @brief       dllPause                         dll��ͣ
*/
extern "C" SIMCONTROL_EXPORT int dllPause();

/**
* @brief       dllContinue                      dll����
*/
extern "C" SIMCONTROL_EXPORT int dllContinue();

/**
* @brief       dllEnd                           dllֹͣ
*/
extern "C" SIMCONTROL_EXPORT int dllEnd();
