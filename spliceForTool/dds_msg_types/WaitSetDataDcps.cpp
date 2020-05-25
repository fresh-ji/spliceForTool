//******************************************************************
// 
//  Generated by IDL to C++ Translator
//  
//  File name: WaitSetDataDcps.cpp
//  Source: E:/2y2b_cmake/spliceForTool/spliceForTool/dds/dds_msg_types/WaitSetDataDcps.idl
//  Generated: Fri Mar 20 14:26:33 2020
//  OpenSplice 6.9.181018OSOS
//  
//******************************************************************

#include "WaitSetDataDcps.h"

#if DDS_USE_EXPLICIT_TEMPLATES
template class DDS_DCPSUVLSeq < WaitSetData::Msg, struct MsgSeq_uniq_>;
#endif

const char * WaitSetData::MsgTypeSupportInterface::_local_id = "IDL:WaitSetData/MsgTypeSupportInterface:1.0";

WaitSetData::MsgTypeSupportInterface_ptr WaitSetData::MsgTypeSupportInterface::_duplicate(WaitSetData::MsgTypeSupportInterface_ptr p)
{
	if (p) p->m_count++;
	return p;
}

DDS::Boolean WaitSetData::MsgTypeSupportInterface::_local_is_a(const char * _id)
{
	if (strcmp(_id, WaitSetData::MsgTypeSupportInterface::_local_id) == 0)
	{
		return true;
	}

	typedef DDS::TypeSupport NestedBase_1;

	if (NestedBase_1::_local_is_a(_id))
	{
		return true;
	}

	return false;
}

WaitSetData::MsgTypeSupportInterface_ptr WaitSetData::MsgTypeSupportInterface::_narrow(DDS::Object_ptr p)
{
	WaitSetData::MsgTypeSupportInterface_ptr result = NULL;
	if (p && p->_is_a(WaitSetData::MsgTypeSupportInterface::_local_id))
	{
		result = dynamic_cast <WaitSetData::MsgTypeSupportInterface_ptr> (p);
		if (result) result->m_count++;
	}
	return result;
}

WaitSetData::MsgTypeSupportInterface_ptr WaitSetData::MsgTypeSupportInterface::_unchecked_narrow(DDS::Object_ptr p)
{
	WaitSetData::MsgTypeSupportInterface_ptr result;
	result = dynamic_cast <WaitSetData::MsgTypeSupportInterface_ptr> (p);
	if (result) result->m_count++;
	return result;
}

const char * WaitSetData::MsgDataWriter::_local_id = "IDL:WaitSetData/MsgDataWriter:1.0";

WaitSetData::MsgDataWriter_ptr WaitSetData::MsgDataWriter::_duplicate(WaitSetData::MsgDataWriter_ptr p)
{
	if (p) p->m_count++;
	return p;
}

DDS::Boolean WaitSetData::MsgDataWriter::_local_is_a(const char * _id)
{
	if (strcmp(_id, WaitSetData::MsgDataWriter::_local_id) == 0)
	{
		return true;
	}

	typedef DDS::DataWriter NestedBase_1;

	if (NestedBase_1::_local_is_a(_id))
	{
		return true;
	}

	return false;
}

WaitSetData::MsgDataWriter_ptr WaitSetData::MsgDataWriter::_narrow(DDS::Object_ptr p)
{
	WaitSetData::MsgDataWriter_ptr result = NULL;
	if (p && p->_is_a(WaitSetData::MsgDataWriter::_local_id))
	{
		result = dynamic_cast <WaitSetData::MsgDataWriter_ptr> (p);
		if (result) result->m_count++;
	}
	return result;
}

WaitSetData::MsgDataWriter_ptr WaitSetData::MsgDataWriter::_unchecked_narrow(DDS::Object_ptr p)
{
	WaitSetData::MsgDataWriter_ptr result;
	result = dynamic_cast <WaitSetData::MsgDataWriter_ptr> (p);
	if (result) result->m_count++;
	return result;
}

const char * WaitSetData::MsgDataReader::_local_id = "IDL:WaitSetData/MsgDataReader:1.0";

WaitSetData::MsgDataReader_ptr WaitSetData::MsgDataReader::_duplicate(WaitSetData::MsgDataReader_ptr p)
{
	if (p) p->m_count++;
	return p;
}

DDS::Boolean WaitSetData::MsgDataReader::_local_is_a(const char * _id)
{
	if (strcmp(_id, WaitSetData::MsgDataReader::_local_id) == 0)
	{
		return true;
	}

	typedef DDS::DataReader NestedBase_1;

	if (NestedBase_1::_local_is_a(_id))
	{
		return true;
	}

	return false;
}

WaitSetData::MsgDataReader_ptr WaitSetData::MsgDataReader::_narrow(DDS::Object_ptr p)
{
	WaitSetData::MsgDataReader_ptr result = NULL;
	if (p && p->_is_a(WaitSetData::MsgDataReader::_local_id))
	{
		result = dynamic_cast <WaitSetData::MsgDataReader_ptr> (p);
		if (result) result->m_count++;
	}
	return result;
}

WaitSetData::MsgDataReader_ptr WaitSetData::MsgDataReader::_unchecked_narrow(DDS::Object_ptr p)
{
	WaitSetData::MsgDataReader_ptr result;
	result = dynamic_cast <WaitSetData::MsgDataReader_ptr> (p);
	if (result) result->m_count++;
	return result;
}

const char * WaitSetData::MsgDataReaderView::_local_id = "IDL:WaitSetData/MsgDataReaderView:1.0";

WaitSetData::MsgDataReaderView_ptr WaitSetData::MsgDataReaderView::_duplicate(WaitSetData::MsgDataReaderView_ptr p)
{
	if (p) p->m_count++;
	return p;
}

DDS::Boolean WaitSetData::MsgDataReaderView::_local_is_a(const char * _id)
{
	if (strcmp(_id, WaitSetData::MsgDataReaderView::_local_id) == 0)
	{
		return true;
	}

	typedef DDS::DataReaderView NestedBase_1;

	if (NestedBase_1::_local_is_a(_id))
	{
		return true;
	}

	return false;
}

WaitSetData::MsgDataReaderView_ptr WaitSetData::MsgDataReaderView::_narrow(DDS::Object_ptr p)
{
	WaitSetData::MsgDataReaderView_ptr result = NULL;
	if (p && p->_is_a(WaitSetData::MsgDataReaderView::_local_id))
	{
		result = dynamic_cast <WaitSetData::MsgDataReaderView_ptr> (p);
		if (result) result->m_count++;
	}
	return result;
}

WaitSetData::MsgDataReaderView_ptr WaitSetData::MsgDataReaderView::_unchecked_narrow(DDS::Object_ptr p)
{
	WaitSetData::MsgDataReaderView_ptr result;
	result = dynamic_cast <WaitSetData::MsgDataReaderView_ptr> (p);
	if (result) result->m_count++;
	return result;
}



