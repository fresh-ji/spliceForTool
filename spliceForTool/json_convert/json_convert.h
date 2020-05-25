
#ifndef _JSONAPI_
#define _JSONAPI_

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "xml_parse.h"

using namespace rapidjson;
using namespace std;

class JSONapi {

public:
	JSONapi(CSScenarioXML*);
	string ConvertTypeData2Json(string name, void* data);
	char* ConvertJson2TypeData(string name, string data);

private:
	CSScenarioXML *p_xmlParser;
};

#endif
