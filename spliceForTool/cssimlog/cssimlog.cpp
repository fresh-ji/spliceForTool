// cssimlog.cpp : 定义 DLL 应用程序的导出函数。
//

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/async.h"

#include "cssimlog.h"

CSSimLog* CSSimLog::Instance() {
	static CSSimLog inst;
	return &inst;
}

bool CSSimLog::CreateLog(const std::string& log_path) {
	spdlog::set_level(spdlog::level::trace);
	spdlog::flush_every(std::chrono::seconds(2));
	spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] %v");

	auto log = spdlog::basic_logger_mt<spdlog::async_factory>("log",
		log_path);

	log_names_.insert("log");

	if (log) {
		log->info("create asy log success");
		log_path_ = log_path;
		return true;
	}
	return false;
}

void CSSimLog::SetLevel(uint32_t level) {
	spdlog::set_level(spdlog::level::level_enum(level));
}

bool CSSimLog::AddCategory(const std::string& category) {
	auto log = spdlog::get(category);
	if (log) {
		return true;
	}

	auto new_log = spdlog::basic_logger_mt<spdlog::async_factory>(category,
		log_path_);
	if (new_log) {
		return true;
	}
	return false;
}

bool CSSimLog::CloseLog() {

	auto lg = spdlog::get("log");
	if (lg) {
		lg->info("log exit");
	}

	//退出前输出所有日志
	for (auto& p : log_names_) {
		auto log = spdlog::get(p);
		if (log) {
			log->flush();
		}
	}

	spdlog::shutdown();
	return true;
}

void CSSimLog::SetCurrentTime(int current_time) {
	current_time_ = current_time;
}

void CSSimLog::SetCurrentTime(const std::string& current_time) {
	current_time_str_ = current_time;
}

CSSimLog::CSSimLog() {

}

CSSimLog::~CSSimLog() {

}

void CSSimLog::Write(const std::string& name, uint32_t level,
	const std::string& msg, const char *filename_in, int line_in,
	const char *funcname_in) {

	std::shared_ptr<spdlog::logger> lg = spdlog::get(name);
	if (!lg) {
		lg = spdlog::basic_logger_mt<spdlog::async_factory>(name, log_path_);
		log_names_.insert(name);
	}

	std::string file_path = filename_in;
    std::string file_name = file_path.substr(file_path.find_last_of('\\') + 1);

    lg->log(spdlog::level::level_enum(level), "[{0:s}:{1:d}] [{2:s}] {3:s} ",
            file_name, line_in, funcname_in, msg);
}

void CSSimLog::Write(const std::string& category, uint32_t level,
	const std::string& msg) {
	std::shared_ptr<spdlog::logger> lg = spdlog::get(category);
	if (!lg) {
		lg = spdlog::basic_logger_mt<spdlog::async_factory>(category, log_path_);
		log_names_.insert(category);
	}
	std::string time_msg = std::string("[") + current_time_str_ + std::string("] ")
		+ msg;
	lg->log(spdlog::level::level_enum(level), "{0}", time_msg);
}

void CSSIMLOG_EXPORT ModuleLog(const std::string& msg, uint32_t level,
	const std::string& entity_uuid) {
	std::string mesg = "[" + entity_uuid + "] " + msg;
	LogModule("module", level, mesg)
}

bool CSSIMLOG_EXPORT CreateLog(const std::string& log_path) {
	return CSSimLog::Instance()->CreateLog(log_path);
}
