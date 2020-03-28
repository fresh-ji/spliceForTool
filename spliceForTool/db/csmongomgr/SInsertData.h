#pragma once

//#include <any>

#include <iostream>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>

#include <vector>

#include "csmongomgr_export.h"
#include "dds_service/dds_msg_struct.h"

using bsoncxx::builder::basic::document;
using bsoncxx::builder::basic::array;
using bsoncxx::builder::basic::kvp;

class CSMONGOMGR_EXPORT CSInsertData {
 public:
  CSInsertData(bsoncxx::builder::basic::document&& doc);
  CSInsertData();
  CSInsertData(CSInsertData&& other);
  ~CSInsertData();

  template<typename T>
  void InsertBasicParam(std::string&& key, T&& value);
  template<typename T>
  void InsertArray(std::string&& key, const std::vector<T>& value);

  void InsertParam(std::string&& name, CSInsertData&& subdoc);
  void InsertParam(std::string&& name, std::vector<CSInsertData>& subdoc);

  bsoncxx::builder::basic::document& GetDoc();

  bool Topic2Doc(const MsgData& datas);

  /*bool Topic2Doc(const std::unordered_map<std::string,std::string>& topic_params,
                 const std::unordered_map<std::string, std::any>& topic_value);

  bool Doc2Topic(const std::unordered_map<std::string, std::string>& topic_params,
                 std::unordered_map<std::string, std::any>& topic_value);

  static bool DocView2Topic(const bsoncxx::document::view& v,
                            const std::unordered_map<std::string, std::string>&
                            topic_params,
                            std::unordered_map<std::string, std::any>& topic_value);

  void ResetDoc(bsoncxx::builder::basic::document&& doc);

  static array Vec2DocArray(const std::string& type, const std::any& value);
  static void DocArray2Vec(const std::string& type, const std::string& key,
                           const bsoncxx::array::view& value,
                           std::unordered_map<std::string, std::any>& topic_value);*/

 private:
  bsoncxx::builder::basic::document m_topicDoc;
};

template<typename T>
void CSInsertData::InsertBasicParam(std::string&& key, T&& value) {
  m_topicDoc.append(bsoncxx::builder::basic::kvp(key, value));
}

template<typename T>
void CSInsertData::InsertArray(std::string&& key,
                               const std::vector<T>& value) {
  bsoncxx::builder::basic::array values;
  for (auto& v : value) {
    values.append(v);
  }
  m_topicDoc.append(bsoncxx::builder::basic::kvp(key, std::move(values)));
}

template void CSInsertData::InsertBasicParam<int8_t>(std::string&&, int8_t&&);
template void CSInsertData::InsertBasicParam<int16_t>(std::string&&, int16_t&&);
template void CSInsertData::InsertBasicParam<int32_t>(std::string&&, int32_t&&);
template void CSInsertData::InsertBasicParam<int64_t>(std::string&&, int64_t&&);
template void CSInsertData::InsertBasicParam<float>(std::string&&, float&&);
template void CSInsertData::InsertBasicParam<double>(std::string&&, double&&);
template void CSInsertData::InsertBasicParam<std::string>(std::string&&,
    std::string&&);

template
void CSInsertData::InsertArray(std::string&& key,
                               const std::vector<int8_t>& value);
template
void CSInsertData::InsertArray(std::string&& key,
                               const std::vector<int16_t>& value);
template
void CSInsertData::InsertArray(std::string&& key,
                               const std::vector<int32_t>& value);
template
void CSInsertData::InsertArray(std::string&& key,
                               const std::vector<int64_t>& value);
template
void CSInsertData::InsertArray(std::string&& key,
                               const std::vector<float>& value);
template
void CSInsertData::InsertArray(std::string&& key,
                               const std::vector<double>& value);
template
void CSInsertData::InsertArray(std::string&& key,
                               const std::vector<std::string>& value);