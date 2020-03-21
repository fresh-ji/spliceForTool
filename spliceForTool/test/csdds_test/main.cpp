#include "csdds_service_export.h"
#include "dds_service.h"
#include "cssimlog_export.h"
#include "cssimlog.h"

#include <windows.h>
#include <iostream>
#include <filesystem>
#include <thread>
//#include <experimental/filesystem>

using namespace std;

bool process(MsgData data)
{
	std::cout << "=======receive data:===========" << endl;

	std::cout << "*********************" << endl;
	std::cout << "content:" << data.content << endl;
	std::cout << "from:" << data.from << endl;
	std::cout << "subjectId:" << data.subjectId << endl;
	std::cout << "systemId:" << data.systemId << endl;
	std::cout << "time:" << data.time << endl;
	std::cout << "topicName:" << data.topicName << endl;
	std::cout << "*********************" << endl;

	return true;
}


int main(int argc, char** argv) {

  SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);

  char path[MAX_PATH];
  if (GetModuleFileName(NULL, path, MAX_PATH)>0)
  {
	  (*strrchr(path, '\\')) = '\0';//丢掉文件名，得到路径   
  }

  int nLength = MultiByteToWideChar(CP_ACP, 0, path, -1, NULL, NULL);
  std::wstring wszStr_path;
  wszStr_path.resize(nLength);
  LPWSTR lpwszStr = new wchar_t[nLength];
  MultiByteToWideChar(CP_ACP, 0, path, -1, lpwszStr, nLength);
  wszStr_path = lpwszStr;

  //auto current_path = fs::current_path();
  auto str = wszStr_path + std::wstring(
		L"/external/OpenSplice/x64/bin");
  AddDllDirectory(str.c_str());

  std::string pt("initialize_log.txt");
  CSSimLog::Instance()->CreateLog(pt);

  CSDDSService *inst = CSDDSService::Instance();
  inst->Init("dds");
 
  inst->SetCallBack(process);
  inst->StartReceiveData();

  std::cout << "start:" << std::endl;
  
  MsgData data;
  data.content = "json1";
  data.from = "A";
  data.subjectId = 100;
  data.systemId = "id";
  data.time = 1;
  data.topicName = "topic1";

  MsgData data2;
  data2.content = "json2";
  data2.from = "B";
  data2.subjectId = 200;
  data2.systemId = "id";
  data2.time = 2;
  data2.topicName = "topic2";

  std::cout << "=======send data=========" << endl;
  std::cout << "*******data1*************" << endl;
  std::cout << "content:" << data.content << endl;
  std::cout << "from:" << data.from << endl;
  std::cout << "subjectId:" << data.subjectId  << endl;
  std::cout << "systemId:" << data.systemId << endl;
  std::cout << "time:" << data.time << endl;
  std::cout << "topicName:" << data.topicName << endl;
  
  
  std::cout << "*******data2*************" << endl;
  std::cout << "content:" << data2.content << endl;
  std::cout << "from:" << data2.from << endl;
  std::cout << "subjectId:" << data2.subjectId << endl;
  std::cout << "systemId:" << data2.systemId << endl;
  std::cout << "time:" << data2.time << endl;
  std::cout << "topicName:" << data2.topicName << endl;


  inst->write(data);
  //Sleep(2000);
  inst->write(data2);

  /*std::vector<MsgData> receive_datas  = inst->read();
  std::cout << "=======receive data:===========" << endl;
  for (auto d : receive_datas){
	  std::cout << "*********************" << endl;
	  std::cout << "content:" << d.content << endl;
	  std::cout << "from:" << d.from << endl;
	  std::cout << "subjectId:" << d.subjectId << endl;
	  std::cout << "systemId:" << d.systemId << endl;
	  std::cout << "time:" << d.time << endl;
	  std::cout << "topicName:" << d.topicName << endl;
	  std::cout << "*********************" << endl;
  }*/
  
  getchar();
  return 0;
}