
#ifndef CS_SIM_LOG_H
#define CS_SIM_LOG_H

//#ifdef CSSIMLOG_EXPORTS
//#define CSSIMLOG_API __declspec(dllexport)
//#else
//#define CSSIMLOG_API __declspec(dllimport)
//#endif



#include <set>
#include <string>
#include "csilog.h"
#include "cssimlog_export.h"

extern "C"
{
	void CSSIMLOG_API ModuleLog(const std::string& msg, uint32_t level,
		const std::string& entity_uuid);
	bool CSSIMLOG_API CreateLog(const std::string& log_path);

}

#define  Log(category, level, msg) \
CSSimLog::Instance()->Write(category, level, msg, __FILE__, __LINE__, __FUNCTION__);

#define LogModule(category, level, msg)\
CSSimLog::Instance()->Write(category, level, msg);

#define LogSETrace(msg) Log("scenario", 0, msg)
#define LogSEDebug(msg) Log("scenario", 1, msg)
#define LogSEInfo(msg)  Log("scenario", 2, msg)
#define LogSEWarn(msg) Log("scenario", 3, msg)
#define LogSEErr(msg) Log("scenario", 4, msg)
#define LogSECri(msg) Log("scenario", 5, msg)

#define Log2DTrace(msg) Log("2dview", 0, msg)
#define Log2DDebug(msg) Log("2dview", 1, msg)
#define Log2DInfo(msg)  Log("2dview", 2, msg)
#define Log2DWarn(msg) Log("2dview", 3, msg)
#define Log2DErr(msg) Log("2dview", 4, msg)
#define Log2DCri(msg) Log("2dview", 5, msg)

#define LogDDSTrace(msg) Log("dds", 0, msg)
#define LogDDSDebug(msg) Log("dds", 1, msg)
#define LogDDSInfo(msg)  Log("dds", 2, msg)
#define LogDDSWarn(msg) Log("dds", 3, msg)
#define LogDDSErr(msg) Log("dds", 4, msg)
#define LogDDSCri(msg) Log("dds", 5, msg)

#define LogEngTrace(msg) Log("engine", 0, msg)
#define LogEngDebug(msg) Log("engine", 1, msg)
#define LogEngInfo(msg)  Log("engine", 2, msg)
#define LogEngWarn(msg) Log("engine", 3, msg)
#define LogEngErr(msg) Log("engine", 4, msg)
#define LogEngCri(msg) Log("engine", 5, msg)

#define LogDBTrace(msg) Log("database", 0, msg)
#define LogDBDebug(msg) Log("database", 1, msg)
#define LogDBInfo(msg)  Log("database", 2, msg)
#define LogDBWarn(msg) Log("database", 3, msg)
#define LogDBErr(msg) Log("database", 4, msg)
#define LogDBCri(msg) Log("database", 5, msg)

#define LogGisTrace(msg) Log("gis", 0, msg)
#define LogGisDebug(msg) Log("gis", 1, msg)
#define LogGisInfo(msg)  Log("gis", 2, msg)
#define LogGisWarn(msg) Log("gis", 3, msg)
#define LogGisErr(msg) Log("gis", 4, msg)
#define LogGisCri(msg) Log("gis", 5, msg)

class CSSIMLOG_API CSSimLog : public CSILog {
 public:
  static CSSimLog* Instance();
  bool CreateLog(const std::string& log_path) override;
  void SetLevel(uint32_t level) override;
  bool AddCategory(const std::string&) override;
  void Write(const std::string& name, uint32_t level, const std::string& msg,
             const char *filename_in, int line_in, const char *funcname_in) override;
  virtual void Write(const std::string& category, uint32_t level,
                     const std::string& msg) override;
  bool CloseLog();

  void SetCurrentTime(int current_time);
  void SetCurrentTime(const std::string& current_time);

 private:
  CSSimLog();
  ~CSSimLog();
  CSSimLog(const CSSimLog&) = delete;
  CSSimLog& operator=(const CSSimLog&) = delete;

 private:
  std::string log_path_;
  int current_time_ = 0;
  std::string current_time_str_ = "0.000000";

  std::set<std::string> log_names_;
};

#endif