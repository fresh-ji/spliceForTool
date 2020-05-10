
#ifndef CS_SIM_LOG_H
#define CS_SIM_LOG_H

#include <set>
#include <iostream>
#include "csilog.h"
#include "cssimlog_export.h"

#define Log(category, level, msg) \
CSSimLog::Instance()->Write(category, level, msg, __FILE__, __LINE__, __FUNCTION__);

#define LogDDSTrace(msg) Log("dds", 0, msg)
#define LogDDSDebug(msg) Log("dds", 1, msg)
#define LogDDSInfo(msg)  Log("dds", 2, msg)
#define LogDDSWarn(msg) Log("dds", 3, msg)
#define LogDDSErr(msg) Log("dds", 4, msg)
#define LogDDSCri(msg) Log("dds", 5, msg)

class CSSIMLOG_EXPORT CSSimLog : public CSILog {
public:
	static CSSimLog* Instance();

	bool CreateLog(const std::string& log_path) override;

	void Write(const std::string& category, uint32_t level, const std::string& msg,
		const char *filename_in, int line_in, const char *funcname_in) override;

	bool CloseLog();

private:
	CSSimLog();
	~CSSimLog();
	CSSimLog(const CSSimLog&) = delete;
	CSSimLog& operator=(const CSSimLog&) = delete;

private:
	std::string log_names_;
};

#endif
