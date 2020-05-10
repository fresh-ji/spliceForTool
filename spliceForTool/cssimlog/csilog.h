
#ifndef CS_I_LOG_H
#define CS_I_LOG_H

#include <stdint.h>

class CSILog {

public:
	virtual bool CreateLog(const std::string& log_path) = 0;

	virtual void Write(const std::string& category, uint32_t level,
		const std::string& msg, const char *filename_in,
		int line_in, const char *funcname_in) = 0;

	virtual bool CloseLog() = 0;
};

#endif
