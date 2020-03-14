
#ifndef _DDSAPI_
#define _DDSAPI_

#include "WaitSetData_DCPS.hpp"
#include "Interface.h"

class Interface;

using namespace WaitSetData;

class DDSapi {

public:
	DDSapi(Interface* i);
	bool startServerDDS();
	bool publish(string, string);

	dds::core::cond::WaitSet waitSet;

private:
	Interface *i;
	map<string, dds::pub::DataWriter<Msg>> writers;
	map<string, dds::sub::DataReader<Msg>> readers;	
};

#endif
