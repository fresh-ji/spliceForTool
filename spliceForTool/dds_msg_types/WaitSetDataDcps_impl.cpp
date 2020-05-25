#include "WaitSetDataDcps_impl.h"
#include "WaitSetDataSplDcps.h"
//#include "examples_export.h"

extern v_copyin_result
__WaitSetData_Msg__copyIn(
c_base base,
const struct WaitSetData::Msg *from,
struct _WaitSetData_Msg *to);

extern void
__WaitSetData_Msg__copyOut(
const void *_from,
void *_to);

// DDS WaitSetData::Msg TypeSupportMetaHolder Object Body
WaitSetData::MsgTypeSupportMetaHolder::MsgTypeSupportMetaHolder() :
DDS::OpenSplice::TypeSupportMetaHolder("WaitSetData::Msg", "", "")
{
	copyIn = (DDS::OpenSplice::cxxCopyIn) __WaitSetData_Msg__copyIn;
	copyOut = (DDS::OpenSplice::cxxCopyOut) __WaitSetData_Msg__copyOut;
	metaDescriptorArrLength = 4;
	metaDescriptorLength = 362;
	const char *tmp[] = { "<MetaData version=\"1.0.0\"><Module name=\"WaitSetData\"><Struct name=\"Msg\"><Member name=\"subjectId\">",
		"<Long/></Member><Member name=\"systemId\"><String/></Member><Member name=\"time\"><Double/></Member><Member name=\"from\">",
		"<String/></Member><Member name=\"topicName\"><String/></Member><Member name=\"content\"><String/></Member>",
		"</Struct></Module></MetaData>" };
	metaDescriptor = new const char*[metaDescriptorArrLength];
	memcpy(metaDescriptor, tmp, sizeof(tmp));
}

WaitSetData::MsgTypeSupportMetaHolder::~MsgTypeSupportMetaHolder()
{
	// Rely on parent destructor.
}

::DDS::OpenSplice::TypeSupportMetaHolder *
WaitSetData::MsgTypeSupportMetaHolder::clone()
{
	return new WaitSetData::MsgTypeSupportMetaHolder();
}

::DDS::OpenSplice::DataWriter *
WaitSetData::MsgTypeSupportMetaHolder::create_datawriter()
{
	return new WaitSetData::MsgDataWriter_impl();
}

::DDS::OpenSplice::DataReader *
WaitSetData::MsgTypeSupportMetaHolder::create_datareader()
{
	return new WaitSetData::MsgDataReader_impl();
}

::DDS::OpenSplice::DataReaderView *
WaitSetData::MsgTypeSupportMetaHolder::create_view()
{
	return new WaitSetData::MsgDataReaderView_impl();
}

// DDS WaitSetData::Msg TypeSupport Object Body
WaitSetData::MsgTypeSupport::MsgTypeSupport() :
DDS::OpenSplice::TypeSupport()
{
	tsMetaHolder = new WaitSetData::MsgTypeSupportMetaHolder();
}

WaitSetData::MsgTypeSupport::~MsgTypeSupport()
{
	DDS::release(tsMetaHolder);
}

// DDS WaitSetData::Msg DataWriter_impl Object Body
WaitSetData::MsgDataWriter_impl::MsgDataWriter_impl()
{
	// Parent constructor takes care of everything.
}

WaitSetData::MsgDataWriter_impl::~MsgDataWriter_impl()
{
	// Parent destructor takes care of everything.
}

::DDS::ReturnCode_t
WaitSetData::MsgDataWriter_impl::init(
DDS::OpenSplice::Publisher *publisher,
DDS::OpenSplice::DomainParticipant *participant,
const DDS::DataWriterQos &qos,
DDS::OpenSplice::Topic *a_topic,
const char *name,
DDS::OpenSplice::cxxCopyIn copyIn,
DDS::OpenSplice::cxxCopyOut copyOut,
u_writerCopy writerCopy,
void *cdrMarshaler)
{
	return DDS::OpenSplice::FooDataWriter_impl::nlReq_init(
		publisher, participant, qos, a_topic, name, copyIn, copyOut, writerCopy, cdrMarshaler);
}

::DDS::InstanceHandle_t
WaitSetData::MsgDataWriter_impl::register_instance(
const WaitSetData::Msg & instance_data) THROW_ORB_EXCEPTIONS
{
	return DDS::OpenSplice::FooDataWriter_impl::register_instance(&instance_data);
}

::DDS::InstanceHandle_t
WaitSetData::MsgDataWriter_impl::register_instance_w_timestamp(
const Msg & instance_data,
const ::DDS::Time_t & source_timestamp) THROW_ORB_EXCEPTIONS
{
	return DDS::OpenSplice::FooDataWriter_impl::register_instance_w_timestamp(&instance_data, source_timestamp);
}

::DDS::ReturnCode_t
WaitSetData::MsgDataWriter_impl::unregister_instance(
const WaitSetData::Msg & instance_data,
::DDS::InstanceHandle_t handle) THROW_ORB_EXCEPTIONS
{
	return DDS::OpenSplice::FooDataWriter_impl::unregister_instance(&instance_data, handle);
}

::DDS::ReturnCode_t
WaitSetData::MsgDataWriter_impl::unregister_instance_w_timestamp(
const Msg & instance_data,
::DDS::InstanceHandle_t handle,
const ::DDS::Time_t & source_timestamp) THROW_ORB_EXCEPTIONS
{
	return DDS::OpenSplice::FooDataWriter_impl::unregister_instance_w_timestamp(&instance_data, handle, source_timestamp);
}

::DDS::ReturnCode_t
WaitSetData::MsgDataWriter_impl::write(
const WaitSetData::Msg & instance_data,
::DDS::InstanceHandle_t handle) THROW_ORB_EXCEPTIONS
{
	return DDS::OpenSplice::FooDataWriter_impl::write(&instance_data, handle);
}

::DDS::ReturnCode_t
WaitSetData::MsgDataWriter_impl::write_w_timestamp(
const Msg & instance_data,
::DDS::InstanceHandle_t handle,
const ::DDS::Time_t & source_timestamp) THROW_ORB_EXCEPTIONS
{
	return DDS::OpenSplice::FooDataWriter_impl::write_w_timestamp(&instance_data, handle, source_timestamp);
}

::DDS::ReturnCode_t
WaitSetData::MsgDataWriter_impl::dispose(
const WaitSetData::Msg & instance_data,
::DDS::InstanceHandle_t handle) THROW_ORB_EXCEPTIONS
{
	return DDS::OpenSplice::FooDataWriter_impl::dispose(&instance_data, handle);
}

::DDS::ReturnCode_t
WaitSetData::MsgDataWriter_impl::dispose_w_timestamp(
const Msg & instance_data,
::DDS::InstanceHandle_t handle,
const ::DDS::Time_t & source_timestamp) THROW_ORB_EXCEPTIONS
{
	return DDS::OpenSplice::FooDataWriter_impl::dispose_w_timestamp(&instance_data, handle, source_timestamp);
}

::DDS::ReturnCode_t
WaitSetData::MsgDataWriter_impl::writedispose(
const WaitSetData::Msg & instance_data,
::DDS::InstanceHandle_t handle) THROW_ORB_EXCEPTIONS
{
	return DDS::OpenSplice::FooDataWriter_impl::writedispose(&instance_data, handle);
}

::DDS::ReturnCode_t
WaitSetData::MsgDataWriter_impl::writedispose_w_timestamp(
const Msg & instance_data,
::DDS::InstanceHandle_t handle,
const ::DDS::Time_t & source_timestamp) THROW_ORB_EXCEPTIONS
{
	return DDS::OpenSplice::FooDataWriter_impl::writedispose_w_timestamp(&instance_data, handle, source_timestamp);
}

::DDS::ReturnCode_t
WaitSetData::MsgDataWriter_impl::get_key_value(
Msg & key_holder,
::DDS::InstanceHandle_t handle) THROW_ORB_EXCEPTIONS
{
	return DDS::OpenSplice::FooDataWriter_impl::get_key_value(&key_holder, handle);
}

::DDS::InstanceHandle_t
WaitSetData::MsgDataWriter_impl::lookup_instance(
const WaitSetData::Msg & instance_data) THROW_ORB_EXCEPTIONS
{
	return DDS::OpenSplice::FooDataWriter_impl::lookup_instance(&instance_data);
}

// DDS WaitSetData::Msg DataReader_impl Object Body
WaitSetData::MsgDataReader_impl::MsgDataReader_impl()
{
	// Parent constructor takes care of everything.
}

WaitSetData::MsgDataReader_impl::~MsgDataReader_impl()
{
	// Parent destructor takes care of everything.
}

DDS::ReturnCode_t
WaitSetData::MsgDataReader_impl::init(
DDS::OpenSplice::Subscriber *subscriber,
const DDS::DataReaderQos &qos,
DDS::OpenSplice::TopicDescription *a_topic,
const char *name,
DDS::OpenSplice::cxxCopyIn copyIn,
DDS::OpenSplice::cxxCopyOut copyOut,
DDS::OpenSplice::cxxReaderCopy readerCopy,
void *cdrMarshaler)
{
	return DDS::OpenSplice::FooDataReader_impl::nlReq_init(
		subscriber, qos, a_topic, name, copyIn, copyOut, readerCopy, cdrMarshaler,
		this->dataSeqAlloc, this->dataSeqLength, this->dataSeqGetBuffer, this->dataSeqCopyOut);
}

::DDS::ReturnCode_t
WaitSetData::MsgDataReader_impl::read(
WaitSetData::MsgSeq & received_data,
::DDS::SampleInfoSeq & info_seq,
::DDS::Long max_samples,
::DDS::SampleStateMask sample_states,
::DDS::ViewStateMask view_states,
::DDS::InstanceStateMask instance_states) THROW_ORB_EXCEPTIONS
{
	::DDS::ReturnCode_t status = DDS::RETCODE_OK;

	status = check_preconditions(received_data, info_seq, max_samples);
	if (status == ::DDS::RETCODE_OK) {
		status = DDS::OpenSplice::FooDataReader_impl::read(&received_data, info_seq, max_samples, sample_states, view_states, instance_states);
	}
	return status;
}

::DDS::ReturnCode_t
WaitSetData::MsgDataReader_impl::take(
WaitSetData::MsgSeq & received_data,
::DDS::SampleInfoSeq & info_seq,
::DDS::Long max_samples,
::DDS::SampleStateMask sample_states,
::DDS::ViewStateMask view_states,
::DDS::InstanceStateMask instance_states) THROW_ORB_EXCEPTIONS
{
	::DDS::ReturnCode_t status = DDS::RETCODE_OK;

	status = check_preconditions(received_data, info_seq, max_samples);
	if (status == ::DDS::RETCODE_OK) {
		status = DDS::OpenSplice::FooDataReader_impl::take(&received_data, info_seq, max_samples, sample_states, view_states, instance_states);
	}
	return status;
}

::DDS::ReturnCode_t
WaitSetData::MsgDataReader_impl::read_w_condition(
WaitSetData::MsgSeq & received_data,
::DDS::SampleInfoSeq & info_seq,
::DDS::Long max_samples,
::DDS::ReadCondition_ptr a_condition) THROW_ORB_EXCEPTIONS
{
	::DDS::ReturnCode_t status = DDS::RETCODE_OK;

	status = check_preconditions(received_data, info_seq, max_samples);
	if (status == ::DDS::RETCODE_OK) {
		status = DDS::OpenSplice::FooDataReader_impl::read_w_condition(&received_data, info_seq, max_samples, a_condition);
	}
	return status;
}

::DDS::ReturnCode_t
WaitSetData::MsgDataReader_impl::take_w_condition(
WaitSetData::MsgSeq & received_data,
::DDS::SampleInfoSeq & info_seq,
::DDS::Long max_samples,
::DDS::ReadCondition_ptr a_condition) THROW_ORB_EXCEPTIONS
{
	::DDS::ReturnCode_t status = DDS::RETCODE_OK;

	status = check_preconditions(received_data, info_seq, max_samples);
	if (status == ::DDS::RETCODE_OK) {
		status = DDS::OpenSplice::FooDataReader_impl::take_w_condition(&received_data, info_seq, max_samples, a_condition);
	}
	return status;
}

::DDS::ReturnCode_t
WaitSetData::MsgDataReader_impl::read_next_sample(
WaitSetData::Msg & received_data,
::DDS::SampleInfo & sample_info) THROW_ORB_EXCEPTIONS
{
	return DDS::OpenSplice::FooDataReader_impl::read_next_sample(&received_data, sample_info);
}

::DDS::ReturnCode_t
WaitSetData::MsgDataReader_impl::take_next_sample(
WaitSetData::Msg & received_data,
::DDS::SampleInfo & sample_info) THROW_ORB_EXCEPTIONS
{
	return DDS::OpenSplice::FooDataReader_impl::take_next_sample(&received_data, sample_info);
}

::DDS::ReturnCode_t
WaitSetData::MsgDataReader_impl::read_instance(
WaitSetData::MsgSeq & received_data,
::DDS::SampleInfoSeq & info_seq,
::DDS::Long max_samples,
::DDS::InstanceHandle_t a_handle,
::DDS::SampleStateMask sample_states,
::DDS::ViewStateMask view_states,
::DDS::InstanceStateMask instance_states) THROW_ORB_EXCEPTIONS
{
	::DDS::ReturnCode_t status;

	status = check_preconditions(received_data, info_seq, max_samples);
	if (status == ::DDS::RETCODE_OK) {
		status = DDS::OpenSplice::FooDataReader_impl::read_instance(&received_data, info_seq, max_samples, a_handle, sample_states, view_states, instance_states);
	}
	return status;
}

::DDS::ReturnCode_t
WaitSetData::MsgDataReader_impl::take_instance(
WaitSetData::MsgSeq & received_data,
::DDS::SampleInfoSeq & info_seq,
::DDS::Long max_samples,
::DDS::InstanceHandle_t a_handle,
::DDS::SampleStateMask sample_states,
::DDS::ViewStateMask view_states,
::DDS::InstanceStateMask instance_states) THROW_ORB_EXCEPTIONS
{
	::DDS::ReturnCode_t status;

	status = check_preconditions(received_data, info_seq, max_samples);
	if (status == ::DDS::RETCODE_OK) {
		status = DDS::OpenSplice::FooDataReader_impl::take_instance(&received_data, info_seq, max_samples, a_handle, sample_states, view_states, instance_states);
	}
	return status;
}

::DDS::ReturnCode_t
WaitSetData::MsgDataReader_impl::read_next_instance(
WaitSetData::MsgSeq & received_data,
::DDS::SampleInfoSeq & info_seq,
::DDS::Long max_samples,
::DDS::InstanceHandle_t a_handle,
::DDS::SampleStateMask sample_states,
::DDS::ViewStateMask view_states,
::DDS::InstanceStateMask instance_states) THROW_ORB_EXCEPTIONS
{
	::DDS::ReturnCode_t status;

	status = check_preconditions(received_data, info_seq, max_samples);
	if (status == ::DDS::RETCODE_OK) {
		status = DDS::OpenSplice::FooDataReader_impl::read_next_instance(&received_data, info_seq, max_samples, a_handle, sample_states, view_states, instance_states);
	}
	return status;
}

::DDS::ReturnCode_t
WaitSetData::MsgDataReader_impl::take_next_instance(
WaitSetData::MsgSeq & received_data,
::DDS::SampleInfoSeq & info_seq,
::DDS::Long max_samples,
::DDS::InstanceHandle_t a_handle,
::DDS::SampleStateMask sample_states,
::DDS::ViewStateMask view_states,
::DDS::InstanceStateMask instance_states) THROW_ORB_EXCEPTIONS
{
	::DDS::ReturnCode_t status;

	status = check_preconditions(received_data, info_seq, max_samples);
	if (status == ::DDS::RETCODE_OK) {
		status = DDS::OpenSplice::FooDataReader_impl::take_next_instance(&received_data, info_seq, max_samples, a_handle, sample_states, view_states, instance_states);
	}
	return status;
}

::DDS::ReturnCode_t
WaitSetData::MsgDataReader_impl::read_next_instance_w_condition(
WaitSetData::MsgSeq & received_data,
::DDS::SampleInfoSeq & info_seq,
::DDS::Long max_samples,
::DDS::InstanceHandle_t a_handle,
::DDS::ReadCondition_ptr a_condition) THROW_ORB_EXCEPTIONS
{
	::DDS::ReturnCode_t status;

	status = check_preconditions(received_data, info_seq, max_samples);
	if (status == ::DDS::RETCODE_OK) {
		status = DDS::OpenSplice::FooDataReader_impl::read_next_instance_w_condition(&received_data, info_seq, max_samples, a_handle, a_condition);
	}
	return status;
}

::DDS::ReturnCode_t
WaitSetData::MsgDataReader_impl::take_next_instance_w_condition(
WaitSetData::MsgSeq & received_data,
::DDS::SampleInfoSeq & info_seq,
::DDS::Long max_samples,
::DDS::InstanceHandle_t a_handle,
::DDS::ReadCondition_ptr a_condition) THROW_ORB_EXCEPTIONS
{
	::DDS::ReturnCode_t status;

	status = check_preconditions(received_data, info_seq, max_samples);
	if (status == ::DDS::RETCODE_OK) {
		status = DDS::OpenSplice::FooDataReader_impl::take_next_instance_w_condition(&received_data, info_seq, max_samples, a_handle, a_condition);
	}
	return status;
}

::DDS::ReturnCode_t
WaitSetData::MsgDataReader_impl::return_loan(
WaitSetData::MsgSeq & received_data,
::DDS::SampleInfoSeq & info_seq) THROW_ORB_EXCEPTIONS
{
	::DDS::ReturnCode_t result = ::DDS::RETCODE_OK;

	result = this->write_lock();
	if (result == DDS::RETCODE_OK) {
		if (received_data.length() > 0) {
			if (received_data.length() == info_seq.length() &&
				received_data.release() == info_seq.release()) {
				if (!received_data.release()) {
					result = DDS::OpenSplice::FooDataReader_impl::wlReq_return_loan(received_data.get_buffer(),
						info_seq.get_buffer());
					if (result == ::DDS::RETCODE_OK) {
						if (!received_data.release()) {
							WaitSetData::MsgSeq::freebuf(received_data.get_buffer(false));
							received_data.replace(0, 0, NULL, false);
							::DDS::SampleInfoSeq::freebuf(info_seq.get_buffer(false));
							info_seq.replace(0, 0, NULL, false);
						}
					}
				}
			}
			else {
				result = ::DDS::RETCODE_PRECONDITION_NOT_MET;
			}
		}
		this->unlock();
	}
	return result;
}

::DDS::ReturnCode_t
WaitSetData::MsgDataReader_impl::get_key_value(
WaitSetData::Msg & key_holder,
::DDS::InstanceHandle_t handle) THROW_ORB_EXCEPTIONS
{
	return DDS::OpenSplice::FooDataReader_impl::get_key_value(&key_holder, handle);
}

::DDS::InstanceHandle_t
WaitSetData::MsgDataReader_impl::lookup_instance(
const WaitSetData::Msg & instance) THROW_ORB_EXCEPTIONS
{
	return DDS::OpenSplice::FooDataReader_impl::lookup_instance(&instance);
}

::DDS::ReturnCode_t
WaitSetData::MsgDataReader_impl::check_preconditions(
WaitSetData::MsgSeq & received_data,
::DDS::SampleInfoSeq & info_seq,
::DDS::Long max_samples)
{
	if ((max_samples < 0) && (max_samples != DDS::LENGTH_UNLIMITED)) {
		return DDS::RETCODE_BAD_PARAMETER;
	}

	/* Rule 1 : The values of len, max_len, and owns
	 * for the two collections must be identical.
	 */
	if ((received_data.length() != info_seq.length()) ||
		(received_data.maximum() != info_seq.maximum()) ||
		(received_data.release() != info_seq.release()))
	{
		return DDS::RETCODE_PRECONDITION_NOT_MET;
	}

	/* Rule 4: If the input max_len>0 and the input owns==FALSE,
	 * then the read operation will fail.
	 */
	if ((info_seq.maximum() > 0) &&
		(info_seq.release() == false))
	{
		return DDS::RETCODE_PRECONDITION_NOT_MET;
	}

	/* Rule 5: If input max_len>0 and the input owns==TRUE,
	 * then the read operation will...
	 */
	if (info_seq.maximum() > 0) { /* owns is true, because of rule 4. */
		/* Rule 5a: If max_samples = LENGTH_UNLIMITED,
		 * then at most max_len values will be copied.
		 * Rule 5b: If max_samples <= max_len,
		 * then at most max_samples values will be copied.
		 */
		/* Rule 5c: If max_samples > max_len,
		 * then the read operation will fail.
		 */
		if ((max_samples != DDS::LENGTH_UNLIMITED) &&
			((DDS::ULong)max_samples > info_seq.maximum())) {
			return DDS::RETCODE_PRECONDITION_NOT_MET;
		}
	}

	if ((max_samples == 0) ||
		((info_seq.maximum() == 0) && (info_seq.release()))) {
		return DDS::RETCODE_NO_DATA;
	}

	return DDS::RETCODE_OK;
}

void *
WaitSetData::MsgDataReader_impl::dataSeqAlloc(
void * received_data,
DDS::ULong len)
{
	WaitSetData::MsgSeq *data_seq = reinterpret_cast<WaitSetData::MsgSeq *>(received_data);
	data_seq->replace(len, len, data_seq->allocbuf(len), false);
	return data_seq->get_buffer();
}

void *
WaitSetData::MsgDataReader_impl::dataSeqGetBuffer(
void * received_data,
DDS::ULong index)
{
	WaitSetData::MsgSeq *data_seq = reinterpret_cast<WaitSetData::MsgSeq *>(received_data);
	return &((*data_seq)[index]);
}

void
WaitSetData::MsgDataReader_impl::dataSeqLength(
void * received_data,
DDS::ULong len)
{
	WaitSetData::MsgSeq *data_seq = reinterpret_cast<WaitSetData::MsgSeq *>(received_data);
	data_seq->length(len);
}

void
WaitSetData::MsgDataReader_impl::dataSeqCopyOut(
const void * from,
void * to)
{
	WaitSetData::Msg *data = reinterpret_cast<WaitSetData::Msg *>(to);
	__WaitSetData_Msg__copyOut(from, data);
}

void
WaitSetData::MsgDataReader_impl::copyDataOut(
const void * from,
void * to)
{
	WaitSetData::Msg *data = reinterpret_cast<WaitSetData::Msg *>(to);
	__WaitSetData_Msg__copyOut(from, data);
}


// DDS WaitSetData::Msg DataReaderView_impl Object Body
WaitSetData::MsgDataReaderView_impl::MsgDataReaderView_impl()
{
	// Parent constructor takes care of everything.
}

WaitSetData::MsgDataReaderView_impl::~MsgDataReaderView_impl()
{
	// Parent destructor takes care of everything.
}

DDS::ReturnCode_t
WaitSetData::MsgDataReaderView_impl::init(
DDS::OpenSplice::DataReader *reader,
const char *name,
const DDS::DataReaderViewQos &qos,
DDS::OpenSplice::cxxCopyIn copyIn,
DDS::OpenSplice::cxxCopyOut copyOut)
{
	return DDS::OpenSplice::FooDataReaderView_impl::nlReq_init(
		reader, name, qos, copyIn, copyOut, WaitSetData::MsgDataReader_impl::dataSeqAlloc,
		WaitSetData::MsgDataReader_impl::dataSeqLength);
}

::DDS::ReturnCode_t
WaitSetData::MsgDataReaderView_impl::read(
WaitSetData::MsgSeq & received_data,
::DDS::SampleInfoSeq & info_seq,
::DDS::Long max_samples,
::DDS::SampleStateMask sample_states,
::DDS::ViewStateMask view_states,
::DDS::InstanceStateMask instance_states) THROW_ORB_EXCEPTIONS
{
	::DDS::ReturnCode_t status;

	status = WaitSetData::MsgDataReader_impl::check_preconditions(received_data, info_seq, max_samples);
	if (status == ::DDS::RETCODE_OK) {
		status = DDS::OpenSplice::FooDataReaderView_impl::read(&received_data, info_seq, max_samples, sample_states, view_states, instance_states);
	}
	return status;
}

::DDS::ReturnCode_t
WaitSetData::MsgDataReaderView_impl::take(
WaitSetData::MsgSeq & received_data,
::DDS::SampleInfoSeq & info_seq,
::DDS::Long max_samples,
::DDS::SampleStateMask sample_states,
::DDS::ViewStateMask view_states,
::DDS::InstanceStateMask instance_states) THROW_ORB_EXCEPTIONS
{
	::DDS::ReturnCode_t status;

	status = WaitSetData::MsgDataReader_impl::check_preconditions(received_data, info_seq, max_samples);
	if (status == ::DDS::RETCODE_OK) {
		status = DDS::OpenSplice::FooDataReaderView_impl::take(&received_data, info_seq, max_samples, sample_states, view_states, instance_states);
	}
	return status;
}

::DDS::ReturnCode_t
WaitSetData::MsgDataReaderView_impl::read_w_condition(
WaitSetData::MsgSeq & received_data,
::DDS::SampleInfoSeq & info_seq,
::DDS::Long max_samples,
::DDS::ReadCondition_ptr a_condition) THROW_ORB_EXCEPTIONS
{
	::DDS::ReturnCode_t status;

	status = WaitSetData::MsgDataReader_impl::check_preconditions(received_data, info_seq, max_samples);
	if (status == ::DDS::RETCODE_OK) {
		status = DDS::OpenSplice::FooDataReaderView_impl::read_w_condition(&received_data, info_seq, max_samples, a_condition);
	}
	return status;
}

::DDS::ReturnCode_t
WaitSetData::MsgDataReaderView_impl::take_w_condition(
WaitSetData::MsgSeq & received_data,
::DDS::SampleInfoSeq & info_seq,
::DDS::Long max_samples,
::DDS::ReadCondition_ptr a_condition) THROW_ORB_EXCEPTIONS
{
	::DDS::ReturnCode_t status;

	status = WaitSetData::MsgDataReader_impl::check_preconditions(received_data, info_seq, max_samples);
	if (status == ::DDS::RETCODE_OK) {
		status = DDS::OpenSplice::FooDataReaderView_impl::take_w_condition(&received_data, info_seq, max_samples, a_condition);
	}
	return status;
}

::DDS::ReturnCode_t
WaitSetData::MsgDataReaderView_impl::read_next_sample(
WaitSetData::Msg & received_data,
::DDS::SampleInfo & sample_info) THROW_ORB_EXCEPTIONS
{
	return DDS::OpenSplice::FooDataReaderView_impl::read_next_sample(&received_data, sample_info);
}

::DDS::ReturnCode_t
WaitSetData::MsgDataReaderView_impl::take_next_sample(
WaitSetData::Msg & received_data,
::DDS::SampleInfo & sample_info) THROW_ORB_EXCEPTIONS
{
	return DDS::OpenSplice::FooDataReaderView_impl::take_next_sample(&received_data, sample_info);
}

::DDS::ReturnCode_t
WaitSetData::MsgDataReaderView_impl::read_instance(
WaitSetData::MsgSeq & received_data,
::DDS::SampleInfoSeq & info_seq,
::DDS::Long max_samples,
::DDS::InstanceHandle_t a_handle,
::DDS::SampleStateMask sample_states,
::DDS::ViewStateMask view_states,
::DDS::InstanceStateMask instance_states) THROW_ORB_EXCEPTIONS
{
	::DDS::ReturnCode_t status;

	status = WaitSetData::MsgDataReader_impl::check_preconditions(received_data, info_seq, max_samples);
	if (status == ::DDS::RETCODE_OK) {
		status = DDS::OpenSplice::FooDataReaderView_impl::read_instance(&received_data, info_seq, max_samples, a_handle, sample_states, view_states, instance_states);
	}
	return status;
}

::DDS::ReturnCode_t
WaitSetData::MsgDataReaderView_impl::take_instance(
WaitSetData::MsgSeq & received_data,
::DDS::SampleInfoSeq & info_seq,
::DDS::Long max_samples,
::DDS::InstanceHandle_t a_handle,
::DDS::SampleStateMask sample_states,
::DDS::ViewStateMask view_states,
::DDS::InstanceStateMask instance_states) THROW_ORB_EXCEPTIONS
{
	::DDS::ReturnCode_t status;

	status = WaitSetData::MsgDataReader_impl::check_preconditions(received_data, info_seq, max_samples);
	if (status == ::DDS::RETCODE_OK) {
		status = DDS::OpenSplice::FooDataReaderView_impl::take_instance(&received_data, info_seq, max_samples, a_handle, sample_states, view_states, instance_states);
	}
	return status;
}

::DDS::ReturnCode_t
WaitSetData::MsgDataReaderView_impl::read_next_instance(
WaitSetData::MsgSeq & received_data,
::DDS::SampleInfoSeq & info_seq,
::DDS::Long max_samples,
::DDS::InstanceHandle_t a_handle,
::DDS::SampleStateMask sample_states,
::DDS::ViewStateMask view_states,
::DDS::InstanceStateMask instance_states) THROW_ORB_EXCEPTIONS
{
	::DDS::ReturnCode_t status;

	status = WaitSetData::MsgDataReader_impl::check_preconditions(received_data, info_seq, max_samples);
	if (status == ::DDS::RETCODE_OK) {
		status = DDS::OpenSplice::FooDataReaderView_impl::read_next_instance(&received_data, info_seq, max_samples, a_handle, sample_states, view_states, instance_states);
	}
	return status;
}

::DDS::ReturnCode_t
WaitSetData::MsgDataReaderView_impl::take_next_instance(
WaitSetData::MsgSeq & received_data,
::DDS::SampleInfoSeq & info_seq,
::DDS::Long max_samples,
::DDS::InstanceHandle_t a_handle,
::DDS::SampleStateMask sample_states,
::DDS::ViewStateMask view_states,
::DDS::InstanceStateMask instance_states) THROW_ORB_EXCEPTIONS
{
	::DDS::ReturnCode_t status;

	status = WaitSetData::MsgDataReader_impl::check_preconditions(received_data, info_seq, max_samples);
	if (status == ::DDS::RETCODE_OK) {
		status = DDS::OpenSplice::FooDataReaderView_impl::take_next_instance(&received_data, info_seq, max_samples, a_handle, sample_states, view_states, instance_states);
	}
	return status;
}

::DDS::ReturnCode_t
WaitSetData::MsgDataReaderView_impl::read_next_instance_w_condition(
WaitSetData::MsgSeq & received_data,
::DDS::SampleInfoSeq & info_seq,
::DDS::Long max_samples,
::DDS::InstanceHandle_t a_handle,
::DDS::ReadCondition_ptr a_condition) THROW_ORB_EXCEPTIONS
{
	::DDS::ReturnCode_t status;

	status = WaitSetData::MsgDataReader_impl::check_preconditions(received_data, info_seq, max_samples);
	if (status == ::DDS::RETCODE_OK) {
		status = DDS::OpenSplice::FooDataReaderView_impl::read_next_instance_w_condition(&received_data, info_seq, max_samples, a_handle, a_condition);
	}
	return status;
}

::DDS::ReturnCode_t
WaitSetData::MsgDataReaderView_impl::take_next_instance_w_condition(
WaitSetData::MsgSeq & received_data,
::DDS::SampleInfoSeq & info_seq,
::DDS::Long max_samples,
::DDS::InstanceHandle_t a_handle,
::DDS::ReadCondition_ptr a_condition) THROW_ORB_EXCEPTIONS
{
	::DDS::ReturnCode_t status;

	status = WaitSetData::MsgDataReader_impl::check_preconditions(received_data, info_seq, max_samples);
	if (status == ::DDS::RETCODE_OK) {
		status = DDS::OpenSplice::FooDataReaderView_impl::take_next_instance_w_condition(&received_data, info_seq, max_samples, a_handle, a_condition);
	}
	return status;
}

::DDS::ReturnCode_t
WaitSetData::MsgDataReaderView_impl::return_loan(
WaitSetData::MsgSeq & received_data,
::DDS::SampleInfoSeq & info_seq) THROW_ORB_EXCEPTIONS
{
	::DDS::ReturnCode_t result = ::DDS::RETCODE_OK;

	result = this->write_lock();
	if (result == DDS::RETCODE_OK) {
		if (received_data.length() > 0) {
			if (received_data.length() == info_seq.length() &&
				received_data.release() == info_seq.release()) {
				if (!received_data.release()) {
					result = DDS::OpenSplice::FooDataReaderView_impl::wlReq_return_loan(received_data.get_buffer(),
						info_seq.get_buffer());

					if (result == ::DDS::RETCODE_OK) {
						if (!received_data.release()) {
							WaitSetData::MsgSeq::freebuf(received_data.get_buffer(false));
							received_data.replace(0, 0, NULL, false);
							::DDS::SampleInfoSeq::freebuf(info_seq.get_buffer(false));
							info_seq.replace(0, 0, NULL, false);
						}
					}
					else if (result == ::DDS::RETCODE_NO_DATA) {
						if (received_data.release()) {
							result = ::DDS::RETCODE_OK;
						}
						else {
							result = ::DDS::RETCODE_PRECONDITION_NOT_MET;
						}
					}
				}
			}
			else {
				result = ::DDS::RETCODE_PRECONDITION_NOT_MET;
			}
		}
		this->unlock();
	}
	return result;
}

::DDS::ReturnCode_t
WaitSetData::MsgDataReaderView_impl::get_key_value(
WaitSetData::Msg & key_holder,
::DDS::InstanceHandle_t handle) THROW_ORB_EXCEPTIONS
{
	return DDS::OpenSplice::FooDataReaderView_impl::get_key_value(&key_holder, handle);
}

::DDS::InstanceHandle_t
WaitSetData::MsgDataReaderView_impl::lookup_instance(
const WaitSetData::Msg & instance) THROW_ORB_EXCEPTIONS
{
	return DDS::OpenSplice::FooDataReaderView_impl::lookup_instance(&instance);
}
