
#include "json_convert.h"

#include "cssimlog.h"
#include "cssimlog_export.h"

JSONapi::JSONapi(CSScenarioXML* p_xmlParser) {
	this->p_xmlParser = p_xmlParser;
}

string JSONapi::ConvertTypeData2Json(string topic_name, void* data_ptr) {
	try {
		TopicDefineInfo topic_define_info =
			p_xmlParser->GetTopicDefineInfo(topic_name);
		unordered_map<string, string> params = topic_define_info.params;

		StringBuffer buf;
		Writer<StringBuffer> writer(buf);
		writer.StartObject();

		if (data_ptr == NULL) {
			LogDDSErr("data pointer null");
			return  "";
		}

		for (auto p : params) {
			string param_name = p.first;
			string param_type = p.second;
			if (param_type == "int32_t") {
				int data = *(int*)data_ptr;
				writer.Key(param_name.c_str());
				writer.Int(data);
			}
			else if (param_type == "double") {
				double data = *(double*)data_ptr;
				writer.Key(param_name.c_str());
				writer.Double(data);
			}
			else if (param_type == "string") {
				string data = (char*)data_ptr;
				writer.Key(param_name.c_str());
				writer.String(data.c_str());
			}
			else {
				TypeDefineInfo type_define_info =
					p_xmlParser->GetTypeDefineInfo(param_type);
				unordered_map<string, string> type_params =
					type_define_info.params;
				writer.Key(param_name.c_str());
				writer.StartObject();
				int index = 0;
				for (auto param : type_params) {
					string struct_param_name = param.first;
					string struct_param_type = param.second;
					if (struct_param_type == "int32_t") {
						int data = *((int*)((char*)data_ptr + index));
						index = index + sizeof(int32_t);
						writer.Key(struct_param_name.c_str());
						writer.Int(data);
					}
					else if (struct_param_type == "double") {
						double data = *((double*)((char*)data_ptr + index));
						index = index + sizeof(double);
						writer.Key(struct_param_name.c_str());
						writer.Double(data);
					}
					else if (struct_param_type == "string") {
						char buffer[1024];
						memset(buffer, 0, sizeof(buffer));
						int len = strlen((char*)((char*)data_ptr + index));
						memcpy(buffer, (char*)data_ptr + index, len);
						string data = buffer;
						index = index + len + 1;
						writer.Key(param_name.c_str());
						writer.String(data.c_str());
					}
				}
				writer.EndObject();
			}
		}
		writer.EndObject();
		const char* json_content = buf.GetString();
		string str_data = json_content;
		return str_data;
	}
	catch (exception &e) {
		string er = e.what();
		LogDDSErr("JSONapi::ConvertTypeData2Json " + er);
		return "";
	}
	catch (...) {
		LogDDSErr("JSONapi::ConvertTypeData2Json CATCHED!!");
		return "";
	}
}

char* JSONapi::ConvertJson2TypeData(string topic_name, string data) {
	try {
		TopicDefineInfo topic_define_info =
			p_xmlParser->GetTopicDefineInfo(topic_name);
		unordered_map<string, string> params = topic_define_info.params;

		char *buffer = (char*)malloc(sizeof(char) * 1024);
		memset(buffer, 0, sizeof(buffer));
		int index = 0;

		if (params.size() <= 0) {
			return buffer;
		}

		string json_content = data;
		Document dom;
		if (!dom.Parse(json_content.c_str()).HasParseError()) {
			for (auto p : params) {
				string param_name = p.first;
				string param_type = p.second;
				if (param_type == "int32_t") {
					if (dom.HasMember(param_name.c_str()) && dom[param_name.c_str()].IsInt()) {
						int data = dom[param_name.c_str()].GetInt();
						memcpy(buffer + index, (char*)&data, sizeof(int32_t));
						index = index + sizeof(int32_t);
					}
				}
				else if (param_type == "double") {
					if (dom.HasMember(param_name.c_str()) && dom[param_name.c_str()].IsDouble()) {
						double data = dom[param_name.c_str()].GetDouble();
						memcpy(buffer + index, (char*)&data, sizeof(double));
						index = index + sizeof(double);
					}
				}
				else if (param_type == "string") {
					if (dom.HasMember(param_name.c_str()) && dom[param_name.c_str()].IsString()) {
						string data = dom[param_name.c_str()].GetString();
						int len = data.length();
						char *tmp = new char[len + 1];
						strcpy(tmp, data.c_str());

						memcpy(buffer + index, (char*)tmp, len + 1);
						index = index + len + 1;
					}
				}
				else {
					TypeDefineInfo type_define_info =
						p_xmlParser->GetTypeDefineInfo(param_type);
					unordered_map<string, string> type_params =
						type_define_info.params;
					rapidjson::Value struct_obj;

					if (dom.HasMember(param_name.c_str()) && dom[param_name.c_str()].IsObject()) {
						struct_obj = dom[param_name.c_str()];
					}

					for (auto param : type_params) {
						string struct_param_name = param.first;
						string struct_param_type = param.second;
						if (struct_param_type == "int32_t") {
							if (struct_obj.HasMember(struct_param_name.c_str()) && struct_obj[struct_param_name.c_str()].IsInt()) {
								int data = struct_obj[struct_param_name.c_str()].GetInt();
								memcpy(buffer + index, (char*)&data, sizeof(int32_t));
								index = index + sizeof(int32_t);
							}
						}
						else if (struct_param_type == "double") {
							if (struct_obj.HasMember(struct_param_name.c_str()) && struct_obj[struct_param_name.c_str()].IsDouble()) {
								double data = struct_obj[struct_param_name.c_str()].GetDouble();
								memcpy(buffer + index, (char*)&data, sizeof(double));
								index = index + sizeof(double);
							}
						}
						else if (struct_param_type == "string") {
							if (struct_obj.HasMember(struct_param_name.c_str()) && struct_obj[struct_param_name.c_str()].IsString()) {
								string data = struct_obj[struct_param_name.c_str()].GetString();
								int len = data.length();
								char *tmp = new char[len + 1];
								strcpy(tmp, data.c_str());

								memcpy(buffer + index, (char*)tmp, len + 1);
								index = index + len + 1;
							}
						}
					}
				}
			}
		}
		else {
			LogDDSErr("fail to parse json : " + json_content);
		}
		return buffer;
	}
	catch (exception &e) {
		string er = e.what();
		LogDDSErr("JSONapi::ConvertJson2TypeData " + er);
		return NULL;
	}
	catch (...) {
		LogDDSErr("JSONapi::ConvertJson2TypeData CATCHED!!");
		return NULL;
	}
}
