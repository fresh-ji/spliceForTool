// cssimlog.cpp : 定义 DLL 应用程序的导出函数。
//

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/async.h"

#include "cssimlog.h"

CSSimLog* CSSimLog::Instance() {
	try {
		static CSSimLog inst;
		return &inst;
	}
	catch (...) {
		std::cout << "[ERRLOG] log get instance fail catched" << std::endl;
	}
}

bool CSSimLog::CreateLog(const std::string& log_path) {
	try {
		spdlog::set_level(spdlog::level::trace);
		spdlog::flush_every(std::chrono::seconds(2));
		spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] %v");

		// create
		log_names_ = "log";
		auto log = spdlog::basic_logger_mt<spdlog::async_factory>(
			log_names_, log_path);

		if (log) {
			log->info("create asy log success");
			return true;
		}
		std::cout << "log create fail " << std::endl;
		return false;
	}
	catch (...) {
		std::cout << "[ERRLOG] log create fail catched" << std::endl;
		return false;
	}
}

bool CSSimLog::CloseLog() {
	try {
		// log all
		auto log = spdlog::get(log_names_);
		if (log) {
			log->info("log exit");
			log->flush();
		}
		spdlog::shutdown();
		return true;
	}
	catch (...) {
		std::cout << "[ERRLOG] log close fail catched" << std::endl;
		return false;
	}
}

void CSSimLog::Write(const std::string& category, uint32_t level,
	const std::string& msg, const char *filename_in, int line_in,
	const char *funcname_in) {
	try {
		std::shared_ptr<spdlog::logger> lg = spdlog::get(category);
		if (!lg) {
			// std::cout << "category not exist at writing " << msg << std::endl;
			return;
		}
		std::string file_path = filename_in;
		std::string file_name = file_path.substr(file_path.find_last_of('\\') + 1);
		lg->log(spdlog::level::level_enum(level), "[{0:s}:{1:d}] [{2:s}] {3:s} ",
			file_name, line_in, funcname_in, msg);
	}
	catch (...) {
		std::cout << "[ERRLOG] log write fail catched" << std::endl;
	}
}

CSSimLog::CSSimLog() {
}

CSSimLog::~CSSimLog() {
}
