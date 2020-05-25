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
	catch (std::exception &e) {
		std::string er = e.what();
		std::cout << "CSSimLog::Instance " + er << std::endl;
		return NULL;
	}
	catch (...) {
		std::cout << "CSSimLog::Instance CATCHED!!" << std::endl;
		return NULL;
	}
}

bool CSSimLog::CreateLog(const std::string& log_path) {
	try {
		spdlog::set_level(spdlog::level::trace);
		spdlog::flush_every(std::chrono::seconds(2));
		spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] %v");

		log_names_ = "dds";
		auto log = spdlog::basic_logger_mt<spdlog::async_factory>(
			log_names_, log_path);

		if (!log) {
			std::cout
				<< "CSSimLog::CreateLog log create fail " << std::endl;
			return false;
		}
		return true;
	}
	catch (std::exception &e) {
		std::string er = e.what();
		std::cout << "CSSimLog::CreateLog " + er << std::endl;
		return false;
	}
	catch (...) {
		std::cout << "CSSimLog::CreateLog CATCHED!!" << std::endl;
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
	catch (std::exception &e) {
		std::string er = e.what();
		std::cout << "CSSimLog::CloseLog " + er << std::endl;
		return false;
	}
	catch (...) {
		std::cout << "CSSimLog::CloseLog CATCHED!!" << std::endl;
		return false;
	}
}

void CSSimLog::Write(const std::string& category, uint32_t level,
	const std::string& msg, const char *filename_in, int line_in,
	const char *funcname_in) {
	try {
		std::shared_ptr<spdlog::logger> lg = spdlog::get(category);
		if (!lg) {
			std::cout << "category not exist at writing " << msg << std::endl;
			return;
		}
		std::string file_path = filename_in;
		std::string file_name = file_path.substr(file_path.find_last_of('\\') + 1);
		lg->log(spdlog::level::level_enum(level), "[{0:s}:{1:d}] [{2:s}] {3:s} ",
			file_name, line_in, funcname_in, msg);
	}
	catch (std::exception &e) {
		std::string er = e.what();
		std::cout << "CSSimLog::Write " + er << std::endl;
	}
	catch (...) {
		std::cout << "CSSimLog::Write CATCHED!!" << std::endl;
	}
}

CSSimLog::CSSimLog() {
}

CSSimLog::~CSSimLog() {
}
