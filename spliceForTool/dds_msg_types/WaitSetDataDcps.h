//******************************************************************
// 
//  Generated by IDL to C++ Translator
//  
//  File name: WaitSetDataDcps.h
//  Source: E:/2y2b_cmake/spliceForTool/spliceForTool/dds/dds_msg_types/WaitSetDataDcps.idl
//  Generated: Fri Mar 20 14:26:33 2020
//  OpenSplice 6.9.181018OSOS
//  
//******************************************************************
#ifndef _WAITSETDATADCPS_H_
#define _WAITSETDATADCPS_H_

#include "sacpp_mapping.h"
#include "dds_dcps.h"
#include "WaitSetData.h"
#include "examples_export.h"


namespace WaitSetData
{

	class OS_EXAMPLE_API MsgTypeSupportInterface;

	typedef MsgTypeSupportInterface * MsgTypeSupportInterface_ptr;
	typedef DDS_DCPSInterface_var < MsgTypeSupportInterface> MsgTypeSupportInterface_var;
	typedef DDS_DCPSInterface_out < MsgTypeSupportInterface> MsgTypeSupportInterface_out;


	class OS_EXAMPLE_API MsgDataWriter;

	typedef MsgDataWriter * MsgDataWriter_ptr;
	typedef DDS_DCPSInterface_var < MsgDataWriter> MsgDataWriter_var;
	typedef DDS_DCPSInterface_out < MsgDataWriter> MsgDataWriter_out;


	class OS_EXAMPLE_API MsgDataReader;

	typedef MsgDataReader * MsgDataReader_ptr;
	typedef DDS_DCPSInterface_var < MsgDataReader> MsgDataReader_var;
	typedef DDS_DCPSInterface_out < MsgDataReader> MsgDataReader_out;


	class OS_EXAMPLE_API MsgDataReaderView;

	typedef MsgDataReaderView * MsgDataReaderView_ptr;
	typedef DDS_DCPSInterface_var < MsgDataReaderView> MsgDataReaderView_var;
	typedef DDS_DCPSInterface_out < MsgDataReaderView> MsgDataReaderView_out;

	struct MsgSeq_uniq_ {};
	typedef DDS_DCPSUVLSeq < Msg, struct MsgSeq_uniq_> MsgSeq;
	typedef DDS_DCPSSequence_var < MsgSeq> MsgSeq_var;
	typedef DDS_DCPSSequence_out < MsgSeq> MsgSeq_out;
	class OS_EXAMPLE_API MsgTypeSupportInterface
		:
		virtual public DDS::TypeSupport
	{
	public:
		typedef MsgTypeSupportInterface_ptr _ptr_type;
		typedef MsgTypeSupportInterface_var _var_type;

		static MsgTypeSupportInterface_ptr _duplicate(MsgTypeSupportInterface_ptr obj);
		DDS::Boolean _local_is_a(const char * id);

		static MsgTypeSupportInterface_ptr _narrow(DDS::Object_ptr obj);
		static MsgTypeSupportInterface_ptr _unchecked_narrow(DDS::Object_ptr obj);
		static MsgTypeSupportInterface_ptr _nil() { return 0; }
		static const char * _local_id;
		MsgTypeSupportInterface_ptr _this() { return this; }


	protected:
		MsgTypeSupportInterface() {};
		~MsgTypeSupportInterface() {};
	private:
		MsgTypeSupportInterface(const MsgTypeSupportInterface &);
		MsgTypeSupportInterface & operator = (const MsgTypeSupportInterface &);
	};

	class OS_EXAMPLE_API MsgDataWriter
		:
		virtual public DDS::DataWriter
	{
	public:
		typedef MsgDataWriter_ptr _ptr_type;
		typedef MsgDataWriter_var _var_type;

		static MsgDataWriter_ptr _duplicate(MsgDataWriter_ptr obj);
		DDS::Boolean _local_is_a(const char * id);

		static MsgDataWriter_ptr _narrow(DDS::Object_ptr obj);
		static MsgDataWriter_ptr _unchecked_narrow(DDS::Object_ptr obj);
		static MsgDataWriter_ptr _nil() { return 0; }
		static const char * _local_id;
		MsgDataWriter_ptr _this() { return this; }

		virtual DDS::LongLong register_instance(const Msg& instance_data) = 0;
		virtual DDS::LongLong register_instance_w_timestamp(const Msg& instance_data, const DDS::Time_t& source_timestamp) = 0;
		virtual DDS::Long unregister_instance(const Msg& instance_data, DDS::LongLong handle) = 0;
		virtual DDS::Long unregister_instance_w_timestamp(const Msg& instance_data, DDS::LongLong handle, const DDS::Time_t& source_timestamp) = 0;
		virtual DDS::Long write(const Msg& instance_data, DDS::LongLong handle) = 0;
		virtual DDS::Long write_w_timestamp(const Msg& instance_data, DDS::LongLong handle, const DDS::Time_t& source_timestamp) = 0;
		virtual DDS::Long dispose(const Msg& instance_data, DDS::LongLong handle) = 0;
		virtual DDS::Long dispose_w_timestamp(const Msg& instance_data, DDS::LongLong handle, const DDS::Time_t& source_timestamp) = 0;
		virtual DDS::Long writedispose(const Msg& instance_data, DDS::LongLong handle) = 0;
		virtual DDS::Long writedispose_w_timestamp(const Msg& instance_data, DDS::LongLong handle, const DDS::Time_t& source_timestamp) = 0;
		virtual DDS::Long get_key_value(Msg& key_holder, DDS::LongLong handle) = 0;
		virtual DDS::LongLong lookup_instance(const Msg& instance_data) = 0;

	protected:
		MsgDataWriter() {};
		~MsgDataWriter() {};
	private:
		MsgDataWriter(const MsgDataWriter &);
		MsgDataWriter & operator = (const MsgDataWriter &);
	};

	class OS_EXAMPLE_API MsgDataReader
		:
		virtual public DDS::DataReader
	{
	public:
		typedef MsgDataReader_ptr _ptr_type;
		typedef MsgDataReader_var _var_type;

		static MsgDataReader_ptr _duplicate(MsgDataReader_ptr obj);
		DDS::Boolean _local_is_a(const char * id);

		static MsgDataReader_ptr _narrow(DDS::Object_ptr obj);
		static MsgDataReader_ptr _unchecked_narrow(DDS::Object_ptr obj);
		static MsgDataReader_ptr _nil() { return 0; }
		static const char * _local_id;
		MsgDataReader_ptr _this() { return this; }

		virtual DDS::Long read(MsgSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::ULong sample_states, DDS::ULong view_states, DDS::ULong instance_states) = 0;
		virtual DDS::Long take(MsgSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::ULong sample_states, DDS::ULong view_states, DDS::ULong instance_states) = 0;
		virtual DDS::Long read_w_condition(MsgSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::ReadCondition_ptr a_condition) = 0;
		virtual DDS::Long take_w_condition(MsgSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::ReadCondition_ptr a_condition) = 0;
		virtual DDS::Long read_next_sample(Msg& received_data, DDS::SampleInfo& sample_info) = 0;
		virtual DDS::Long take_next_sample(Msg& received_data, DDS::SampleInfo& sample_info) = 0;
		virtual DDS::Long read_instance(MsgSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::LongLong a_handle, DDS::ULong sample_states, DDS::ULong view_states, DDS::ULong instance_states) = 0;
		virtual DDS::Long take_instance(MsgSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::LongLong a_handle, DDS::ULong sample_states, DDS::ULong view_states, DDS::ULong instance_states) = 0;
		virtual DDS::Long read_next_instance(MsgSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::LongLong a_handle, DDS::ULong sample_states, DDS::ULong view_states, DDS::ULong instance_states) = 0;
		virtual DDS::Long take_next_instance(MsgSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::LongLong a_handle, DDS::ULong sample_states, DDS::ULong view_states, DDS::ULong instance_states) = 0;
		virtual DDS::Long read_next_instance_w_condition(MsgSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::LongLong a_handle, DDS::ReadCondition_ptr a_condition) = 0;
		virtual DDS::Long take_next_instance_w_condition(MsgSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::LongLong a_handle, DDS::ReadCondition_ptr a_condition) = 0;
		virtual DDS::Long return_loan(MsgSeq& received_data, DDS::SampleInfoSeq& info_seq) = 0;
		virtual DDS::Long get_key_value(Msg& key_holder, DDS::LongLong handle) = 0;
		virtual DDS::LongLong lookup_instance(const Msg& instance) = 0;

	protected:
		MsgDataReader() {};
		~MsgDataReader() {};
	private:
		MsgDataReader(const MsgDataReader &);
		MsgDataReader & operator = (const MsgDataReader &);
	};

	class OS_EXAMPLE_API MsgDataReaderView
		:
		virtual public DDS::DataReaderView
	{
	public:
		typedef MsgDataReaderView_ptr _ptr_type;
		typedef MsgDataReaderView_var _var_type;

		static MsgDataReaderView_ptr _duplicate(MsgDataReaderView_ptr obj);
		DDS::Boolean _local_is_a(const char * id);

		static MsgDataReaderView_ptr _narrow(DDS::Object_ptr obj);
		static MsgDataReaderView_ptr _unchecked_narrow(DDS::Object_ptr obj);
		static MsgDataReaderView_ptr _nil() { return 0; }
		static const char * _local_id;
		MsgDataReaderView_ptr _this() { return this; }

		virtual DDS::Long read(MsgSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::ULong sample_states, DDS::ULong view_states, DDS::ULong instance_states) = 0;
		virtual DDS::Long take(MsgSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::ULong sample_states, DDS::ULong view_states, DDS::ULong instance_states) = 0;
		virtual DDS::Long read_w_condition(MsgSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::ReadCondition_ptr a_condition) = 0;
		virtual DDS::Long take_w_condition(MsgSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::ReadCondition_ptr a_condition) = 0;
		virtual DDS::Long read_next_sample(Msg& received_data, DDS::SampleInfo& sample_info) = 0;
		virtual DDS::Long take_next_sample(Msg& received_data, DDS::SampleInfo& sample_info) = 0;
		virtual DDS::Long read_instance(MsgSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::LongLong a_handle, DDS::ULong sample_states, DDS::ULong view_states, DDS::ULong instance_states) = 0;
		virtual DDS::Long take_instance(MsgSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::LongLong a_handle, DDS::ULong sample_states, DDS::ULong view_states, DDS::ULong instance_states) = 0;
		virtual DDS::Long read_next_instance(MsgSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::LongLong a_handle, DDS::ULong sample_states, DDS::ULong view_states, DDS::ULong instance_states) = 0;
		virtual DDS::Long take_next_instance(MsgSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::LongLong a_handle, DDS::ULong sample_states, DDS::ULong view_states, DDS::ULong instance_states) = 0;
		virtual DDS::Long read_next_instance_w_condition(MsgSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::LongLong a_handle, DDS::ReadCondition_ptr a_condition) = 0;
		virtual DDS::Long take_next_instance_w_condition(MsgSeq& received_data, DDS::SampleInfoSeq& info_seq, DDS::Long max_samples, DDS::LongLong a_handle, DDS::ReadCondition_ptr a_condition) = 0;
		virtual DDS::Long return_loan(MsgSeq& received_data, DDS::SampleInfoSeq& info_seq) = 0;
		virtual DDS::Long get_key_value(Msg& key_holder, DDS::LongLong handle) = 0;
		virtual DDS::LongLong lookup_instance(const Msg& instance) = 0;

	protected:
		MsgDataReaderView() {};
		~MsgDataReaderView() {};
	private:
		MsgDataReaderView(const MsgDataReaderView &);
		MsgDataReaderView & operator = (const MsgDataReaderView &);
	};

}




#endif
