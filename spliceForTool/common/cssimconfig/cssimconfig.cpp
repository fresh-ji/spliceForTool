#include "cssimconfig.h"

#include <filesystem>
#include <iostream>
#include <fstream>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "cssimlog/cssimlog.h"

using namespace rapidjson;

CSSimConfig::~CSSimConfig() {

}

CSSimConfig& CSSimConfig::Instance() {
  static CSSimConfig instance_;
  return instance_;
}

bool CSSimConfig::LoadConfig(const std::string& file_path) {

  sim_config_.clear();

  auto file_name = file_path + "/config.json";
  std::ifstream file(file_name);
  if (!file.is_open()) {
      LogSEErr("open config file failed, file path: " + file_name)
      return false;
  }
 
  file.seekg(0, std::ios::end);
  int file_size = file.tellg();
  file.seekg(0, std::ios::beg);

  char *buf = new char[file_size + 1];
  memset(buf, 0, file_size + 1);
  file.read(buf, file_size);
  std::string json_content(buf);
  delete[]buf;
  file.close();


  try {
	  Document dom;
	  std::map<std::string, std::string> mogo_db_config;
	  if (!dom.Parse(json_content.c_str()).HasParseError()) {
		  if (dom.HasMember("MongDBConfig") && dom["MongDBConfig"].IsObject()) {
			  const rapidjson::Value& obj = dom["MongDBConfig"];

			  if (obj.HasMember("ip_address") && obj["ip_address"].IsString()) {
				  std::string ip = obj["ip_address"].GetString();
				  mogo_db_config["ip_address"] = ip;
			  }

			  if (obj.HasMember("port") && obj["port"].IsString()) {
				  std::string port = obj["port"].GetString();
				  mogo_db_config["port"] = port;
			  }
		  }
	  }
	  sim_config_["MongDBConfig"] = mogo_db_config;
  } catch (...) {
    LogSEErr("parse config file failed")
    return false;
  }

  return true;
}

CSSimConfig::CSSimConfig() {
}

const std::map<std::string, std::map<std::string, std::string>> &CSSimConfig::
GetSimConfigs()const {
  return sim_config_;
}

void CSSimConfig::SetSimConfigs(
  std::map<std::string, std::map<std::string, std::string>> &config) {
  sim_config_ = config;
}

std::map<std::string, std::string> CSSimConfig::GetConfig(
const std::string& key) {
  auto value = sim_config_.find(key);
  if (value != sim_config_.end()) {
    return value->second;
  }
  else{
	  std::map<std::string, std::string> empty;
	  return  empty;
  }
  //return std::nullopt;
}

void CSSimConfig::SetConfig(const std::string& key,
                            const std::map<std::string, std::string>& value) {
  sim_config_[key] = value;
}

