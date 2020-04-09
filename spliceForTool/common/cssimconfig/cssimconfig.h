
#ifndef CS_SIMCONFIG_H
#define CS_SIMCONFIG_H

#include "cssimconfig/cssimconfig_export.h"

#include <map>
//#include <optional>

class CSSIMCONFIG_EXPORT CSSimConfig {

 public:
  static CSSimConfig& Instance();

  bool LoadConfig(const std::string& file_path);

  const std::map<std::string, std::map<std::string, std::string>>&
      GetSimConfigs()const;

  void SetSimConfigs(std::map<std::string, std::map<std::string, std::string>>
                     &config);

  std::map<std::string, std::string> GetConfig(
        const std::string& key);

  void SetConfig(const std::string& key,
                 const std::map<std::string,std::string>& value);

 private:
  CSSimConfig();
  ~CSSimConfig();
  CSSimConfig(const CSSimConfig& other) = delete;
  CSSimConfig& operator=(const CSSimConfig& other) = delete;

 private:
  std::map<std::string, std::map<std::string, std::string>> sim_config_;

};

#endif //CS_SIMCONFIG_H