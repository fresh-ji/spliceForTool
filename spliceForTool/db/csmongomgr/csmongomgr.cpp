#include "csmongomgr.h"
#include <iostream>

#include <bsoncxx/json.hpp>
#include <mongocxx/exception/query_exception.hpp>
#include <bsoncxx/stdx/make_unique.hpp>

#include <windows.h>

CSMongoMgr::CSMongoMgr() {
  instance_ = bsoncxx::stdx::make_unique<mongocxx::instance>();
}

CSMongoMgr::~CSMongoMgr() {

}

CSMongoMgr* CSMongoMgr::Instance() {
  static CSMongoMgr inst;
  return &inst;
}

void CSMongoMgr::Insert(std::vector<CSInsertData>& docs) {
  std::vector<bsoncxx::document::view> values;
  for (auto& doc : docs) {
    values.push_back(doc.GetDoc().view());
  }
  db_[table_name_].insert_many(values);
}

void CSMongoMgr::Insert(CSInsertData& doc) {

  try {
    db_[table_name_].insert_one(doc.GetDoc().extract());
  } catch (...) {

  }
}

bool CSMongoMgr::Play(int32_t time) {

  std::cout << "time:" << time << std::endl;
  std::cout << "==========================================" << std::endl;

  auto doc = bsoncxx::builder::basic::make_document(kvp("time",
             std::to_string(time)));

  try {
    bsoncxx::stdx::optional<bsoncxx::document::value> rv =
      db_[table_name_].find_one(doc.view());

    if (!rv.has_value()) {
      return false;
    }

    auto v = rv.value().view();
    for (auto &e : v) {
      if (e.type() != bsoncxx::type::k_array) {
        continue;
      }
      auto data_name = e.key();
      //主题样本数据组成的数组
      bsoncxx::array::view topic_array{ e.get_array().value };
      //遍历主题
      for (const bsoncxx::array::element& topic : topic_array) {
        //每一个主题都是一个doc
        if (topic.type() != bsoncxx::type::k_document) {
          continue;
        }
        auto topic_samples = topic.get_document();
        //遍历主题sample
        for (auto& sam : topic_samples.view()) {
          auto sk = sam.key();
          if (sam.type() != bsoncxx::type::k_array) {
            continue;
          }

          bsoncxx::array::view ss{ sam.get_array().value };
          for (auto& s : ss) {
            //std::cout << bsoncxx::to_json(s.get_document().value) << std::endl;
            cb_(std::string(sk), s.get_document().view());
          }
        }
      }
    }
  } catch (const mongocxx::query_exception& e) {
    return false;
  }
  return true;
}

void CSMongoMgr::SetReplayCallBack(
  std::function<void(const std::string&, const bsoncxx::document::view&)>
  cb) {
  cb_ = cb;
}

std::vector<std::string> CSMongoMgr::GetTables(const std::string& filter) {
  std::vector<std::string> collections;
  try {
    collections = db_.list_collection_names();
  } catch (const mongocxx::operation_exception& e) {

  }
  return collections;
}

bool CSMongoMgr::ConnectDB(const std::string& uri) {
  try {
    mongocxx::uri muri(uri.c_str());
    client_ =  mongocxx::client(muri);
  } catch (const mongocxx::exception& e) {
    std::cout << e.what() << std::endl;
    return false;
  } catch (...) {
    return false;
  }
  return client_.operator bool();
}

bool CSMongoMgr::OpenDB(const std::string& db_name,
                        const std::string& table_name) {
  db_ = client_[db_name];
  table_name_ = table_name;
  return true;
}

