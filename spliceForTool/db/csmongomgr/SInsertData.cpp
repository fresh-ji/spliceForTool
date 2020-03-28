
#include <iostream>

#include "SInsertData.h"
//#include "csstructwrap/csstructwrap.h"
//#include "csdata_types/csdata_types.h"

CSInsertData::CSInsertData() {
}

CSInsertData::CSInsertData(CSInsertData&& other) {
  m_topicDoc = std::move(other.m_topicDoc);
}

CSInsertData::CSInsertData(bsoncxx::builder::basic::document&& doc) {
  m_topicDoc = std::move(doc);
}

CSInsertData::~CSInsertData() {
}

void CSInsertData::InsertParam(std::string&& name, CSInsertData&& subdoc) {
  m_topicDoc.append(bsoncxx::builder::basic::kvp(name, subdoc.GetDoc()));
}

void CSInsertData::InsertParam(std::string&& name,
                               std::vector<CSInsertData>& subdoc) {
  array values;
  for (auto& v : subdoc) {
    values.append(v.GetDoc());
  }
  m_topicDoc.append(bsoncxx::builder::basic::kvp(name, values));
}

bsoncxx::builder::basic::document& CSInsertData::GetDoc() {
  return m_topicDoc;
}


bool CSInsertData::Topic2Doc(const MsgData& data) {
	try {
		m_topicDoc.append(kvp("systemId", data.systemId));
		m_topicDoc.append(kvp("subjectId", data.subjectId));
		m_topicDoc.append(kvp("time", data.time));
		m_topicDoc.append(kvp("topicName", data.topicName));
		m_topicDoc.append(kvp("from", data.from));
		m_topicDoc.append(kvp("content", data.content));

	} catch (...) {
    return false;
	}

	return true;
}

//bool CSInsertData::Topic2Doc(const
//                             std::unordered_map<std::string, std::string>& topic_params,
//                             const std::unordered_map<std::string, std::any>& topic_value) {
//  try {
//
//    for (auto& p : topic_params) {
//      auto& v = topic_value.find(p.first);
//      if (v == topic_value.end()) {
//        return false;
//      }
//
//      if ("int8_t" == p.second) {
//        m_topicDoc.append(kvp(p.first, (int32_t)std::any_cast<int8_t>(v->second)));
//      } else if ("uint8_t" == p.second) {
//        m_topicDoc.append(kvp(p.first, (int32_t)std::any_cast<uint8_t>(v->second)));
//      } else if ("int16_t" == p.second) {
//        m_topicDoc.append(kvp(p.first, (int32_t)std::any_cast<int16_t>(v->second)));
//      } else if ("uint16_t" == p.second) {
//        m_topicDoc.append(kvp(p.first, (int32_t)std::any_cast<uint16_t>(v->second)));
//      } else if ("int32_t" == p.second) {
//        m_topicDoc.append(kvp(p.first, (int32_t)std::any_cast<int32_t>(v->second)));
//      } else if ("uint32_t" == p.second) {
//        m_topicDoc.append(kvp(p.first, (int32_t)std::any_cast<uint32_t>(v->second)));
//      } else if ("int64_t" == p.second) {
//        m_topicDoc.append(kvp(p.first, (int64_t)std::any_cast<int64_t>(v->second)));
//      } else if ("uint64_t" == p.second) {
//        m_topicDoc.append(kvp(p.first, (int64_t)std::any_cast<uint64_t>(v->second)));
//      } else if ("float" == p.second) {
//        m_topicDoc.append(kvp(p.first, (double)std::any_cast<float>(v->second)));
//      } else if ("double" == p.second) {
//        m_topicDoc.append(kvp(p.first, std::any_cast<double>(v->second)));
//      } else if ("string" == p.second) {
//        m_topicDoc.append(kvp(p.first, std::any_cast<std::string>(v->second)));
//      }  else if (std::string::npos != p.second.find("vector")) {
//        m_topicDoc.append(kvp(p.first, Vec2DocArray(p.second, v->second)));
//      } else {
//        //查找不到的类型当作结构体处理
//        CSStructWrap wrap = std::any_cast<CSStructWrap>(v->second);
//        bsoncxx::types::b_binary binary_data = {
//          bsoncxx::binary_sub_type::k_binary, wrap.Length(), wrap.Data()
//        };
//        m_topicDoc.append(kvp(p.first, binary_data));
//      }
//    }
//  } catch (...) {
//    return false;
//  }
//
//  return true;
//}
//
//bool CSInsertData::Doc2Topic(const std::unordered_map<std::string, std::string>&
//                             topic_params, std::unordered_map<std::string, std::any>& topic_value) {
//
//  auto v = m_topicDoc.view();
//  for (auto& p : topic_params) {
//    auto it = v.find(p.first);
//    if (it == v.end()) {
//      break;
//    }
//
//    if ("int8_t" == p.second) {
//      int8_t value = it->get_int32();
//      topic_value.emplace(p.first, value);
//    } else if ("uint8_t" == p.second) {
//      uint8_t value = it->get_int32();
//      topic_value.emplace(p.first, value);
//    } else if ("int16_t" == p.second) {
//      int16_t value = it->get_int32();
//      topic_value.emplace(p.first, value);
//    } else if ("uint16_t" == p.second) {
//      uint16_t value = it->get_int32();
//      topic_value.emplace(p.first, value);
//    } else if ("int32_t" == p.second) {
//      int32_t value = it->get_int32();
//      topic_value.emplace(p.first, value);
//    } else if ("uint32_t" == p.second) {
//      uint32_t value = it->get_int32();
//      topic_value.emplace(p.first, value);
//    } else if ("int64_t" == p.second) {
//      int64_t value = it->get_int64();
//      topic_value.emplace(p.first, value);
//    } else if ("uint64_t" == p.second) {
//      uint64_t value = it->get_int64();
//      topic_value.emplace(p.first, value);
//    } else if ("float" == p.second) {
//      topic_value.emplace(p.first, it->get_double());
//    } else if ("double" == p.second) {
//      topic_value.emplace(p.first, it->get_double());
//    } else if ("string" == p.second) {
//      topic_value.emplace(p.first, std::string(it->get_utf8().value));
//    } else if ("struct" == p.second) {
//      auto data = it->get_binary();
//      CSStructWrap wrap((void*)data.bytes, data.size);
//      topic_value.emplace(p.first, wrap);
//    } else if (std::string::npos != p.second.find("vector")) {
//      DocArray2Vec(p.first, p.second, it->get_array().value, topic_value);
//    }
//  }
//  return true;
//}
//
//bool CSInsertData::DocView2Topic(const bsoncxx::document::view& v,
//                                 const std::unordered_map<std::string, std::string>& topic_params,
//                                 std::unordered_map<std::string, std::any>& topic_value) {
//
//  for (auto& p : topic_params) {
//    auto it = v.find(p.first);
//    if (it == v.end()) {
//      break;
//    }
//
//    if ("int8_t" == p.second) {
//      int8_t value = it->get_int32();
//      topic_value.emplace(p.first, value);
//    } else if ("uint8_t" == p.second) {
//      uint8_t value = it->get_int32();
//      topic_value.emplace(p.first, value);
//    } else if ("int16_t" == p.second) {
//      int16_t value = it->get_int32();
//      topic_value.emplace(p.first, value);
//    } else if ("uint16_t" == p.second) {
//      uint16_t value = it->get_int32();
//      topic_value.emplace(p.first, value);
//    } else if ("int32_t" == p.second) {
//      int32_t value = it->get_int32();
//      topic_value.emplace(p.first, value);
//    } else if ("uint32_t" == p.second) {
//      uint32_t value = it->get_int32();
//      topic_value.emplace(p.first, value);
//    } else if ("int64_t" == p.second) {
//      int64_t value = it->get_int64();
//      topic_value.emplace(p.first, value);
//    } else if ("uint64_t" == p.second) {
//      uint64_t value = it->get_int64();
//      topic_value.emplace(p.first, value);
//    } else if ("float" == p.second) {
//      double db = it->get_double();
//      topic_value.emplace(p.first, db);
//    } else if ("double" == p.second) {
//      double db = it->get_double();
//      topic_value.emplace(p.first, db);
//    } else if ("string" == p.second) {
//      std::string str = std::string(it->get_utf8().value);
//      topic_value.emplace(p.first, str);
//    }  else if (std::string::npos != p.second.find("vector")) {
//      DocArray2Vec(p.first, p.second, it->get_array().value, topic_value);
//    } else {
//      auto data = it->get_binary();
//      CSStructWrap wrap((void*)data.bytes, data.size);
//      topic_value.emplace(p.first, wrap);
//    }
//  }
//  return true;
//}
//
//void CSInsertData::ResetDoc(bsoncxx::builder::basic::document&& doc) {
//  m_topicDoc.clear();
//  m_topicDoc = std::move(doc);
//}
//
//array CSInsertData::Vec2DocArray(const std::string& type,
//                                 const std::any& value) {
//  array ar;
//  //获取模板参数类型
//  auto tpl_name = CSDataTypes::GetTemplateType(type);
//  if ("uint8_t" == tpl_name) {
//    auto v = std::any_cast<std::vector<uint8_t>>(value);
//    for (auto& p : v) {
//      ar.append(p);
//    }
//  } else if ("int8_t" == tpl_name) {
//    auto v = std::any_cast<std::vector<int8_t>>(value);
//    for (auto& p : v) {
//      ar.append(p);
//    }
//  } else if ("uint16_t" == tpl_name) {
//    auto v = std::any_cast<std::vector<uint16_t>>(value);
//    for (auto& p : v) {
//      ar.append(p);
//    }
//  } else if ("int16_t" == tpl_name) {
//    auto v = std::any_cast<std::vector<int16_t>>(value);
//    for (auto& p : v) {
//      ar.append(p);
//    }
//  } else if ("uint32_t" == tpl_name) {
//    auto v = std::any_cast<std::vector<uint32_t>>(value);
//    for (auto& p : v) {
//      ar.append((int32_t)p);
//    }
//  } else if ("int32_t" == tpl_name) {
//    auto v = std::any_cast<std::vector<int32_t>>(value);
//    for (auto& p : v) {
//      ar.append((int32_t)p);
//    }
//  } else if ("uint64_t" == tpl_name) {
//    auto v = std::any_cast<std::vector<uint64_t>>(value);
//    for (auto& p : v) {
//      ar.append((int64_t)p);
//    }
//  } else if ("int64_t" == tpl_name) {
//    auto v = std::any_cast<std::vector<int64_t>>(value);
//    for (auto& p : v) {
//      ar.append((int64_t)p);
//    }
//  } else if ("float" == tpl_name) {
//    auto v = std::any_cast<std::vector<float>>(value);
//    for (auto& p : v) {
//      ar.append(p);
//    }
//  } else if ("double" == tpl_name) {
//    auto v = std::any_cast<std::vector<double>>(value);
//    for (auto& p : v) {
//      ar.append(p);
//    }
//  } else if ("string" == tpl_name) {
//    auto v = std::any_cast<std::vector<std::string>>(value);
//    for (auto& p : v) {
//      ar.append(p);
//    }
//  } else {
//    //结构体类型
//    auto v = std::any_cast<std::vector<CSStructWrap>>(value);
//    for (auto& p : v) {
//      bsoncxx::types::b_binary binary_data = {
//        bsoncxx::binary_sub_type::k_binary, p.Length(), p.Data()
//      };
//      ar.append(binary_data);
//    }
//  }
//  return ar;
//}
//
//void CSInsertData::DocArray2Vec(const std::string& type, const std::string& key,
//                                const bsoncxx::array::view& value,
//                                std::unordered_map<std::string, std::any>& topic_value) {
//  //获取模板参数类型
//
//  auto tpl_name = CSDataTypes::GetTemplateType(type);
//  if ("int8_t" == tpl_name || "uint8_t" == tpl_name || "int16_t" == tpl_name
//      || "uint16_t" == tpl_name || "int32_t" == tpl_name || "uint32_t" == tpl_name
//     ) {
//    std::vector<int32_t> vec;
//    for (auto& e : value) {
//      vec.push_back(e.get_int32());
//    }
//    topic_value.emplace(key, vec);
//  }  else if ("int64_t" == tpl_name || "uint64_t" == tpl_name) {
//    std::vector<uint64_t> vec;
//    for (auto& e : value) {
//      vec.push_back(e.get_int64());
//    }
//    topic_value.emplace(key, vec);
//  } else if ("float" == tpl_name) {
//    std::vector<float> vec;
//    for (auto& e : value) {
//      vec.push_back(e.get_double());
//    }
//    topic_value.emplace(key, vec);
//  } else if ("double" == tpl_name) {
//    std::vector<double> vec;
//    for (auto& e : value) {
//      vec.push_back(e.get_double());
//    }
//    topic_value.emplace(key, vec);
//  } else if ("string" == tpl_name) {
//    std::vector<std::string> vec;
//    for (auto& e : value) {
//      vec.push_back(std::string(e.get_utf8().value));
//    }
//    topic_value.emplace(key, vec);
//  } else {
//    std::vector<CSStructWrap> vec;
//    for (auto& e : value) {
//      vec.emplace_back((void*)e.get_binary().bytes, e.get_binary().size);
//    }
//    topic_value.emplace(key, vec);
//  }
//}

