// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� SPLICEFORTOOL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// SPLICEFORTOOL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef SPLICEFORTOOL_EXPORTS
#define SPLICEFORTOOL_API __declspec(dllexport)
#else
#define SPLICEFORTOOL_API __declspec(dllimport)
#endif

// �����Ǵ� spliceForTool.dll ������
class SPLICEFORTOOL_API CspliceForTool {
public:
	CspliceForTool(void);
	// TODO:  �ڴ�������ķ�����
};

extern SPLICEFORTOOL_API int nspliceForTool;

SPLICEFORTOOL_API int fnspliceForTool(void);
