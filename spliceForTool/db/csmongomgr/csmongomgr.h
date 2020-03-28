#pragma once
#include <iostream>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include "SInsertData.h"
//#include <any>

#include "csmongomgr_export.h"


#define Mongo (CSMongoMgr::Instance())

class CSMONGOMGR_EXPORT CSMongoMgr {

 public:
  ~CSMongoMgr();

  static CSMongoMgr* Instance();

  bool ConnectDB(const std::string& uri);
  bool OpenDB(const std::string& db_name, const std::string& table_name);
  void Insert(std::vector<CSInsertData>& docs);
  void Insert(CSInsertData& doc);

  bool Play(int32_t time);
  void SetReplayCallBack(
    std::function<void(const std::string&, const bsoncxx::document::view&)>
    cb);

  std::vector<std::string> GetTables(const std::string& filter);

 private:
  CSMongoMgr();

 private:
  mongocxx::client client_;
  mongocxx::database db_;
  std::string table_name_;
  std::unique_ptr<mongocxx::instance> instance_ = nullptr;
  std::function<void (const std::string&, const bsoncxx::document::view& )> cb_;
};



