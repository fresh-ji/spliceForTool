// DDSService.cpp : 定义 DLL 应用程序的导出函数。
//

#include <thread>

#include "dds_service.h"
#include "check_status.h"

//msg_type
#include "ccpp_WaitSetData.h"
#include "examples_export.h"

#include "cssimlog/cssimlog.h"

using namespace WaitSetData;

CSDDSService::CSDDSService() {
}

CSDDSService::~CSDDSService() {
}

CSDDSService* CSDDSService::Instance() {
	static CSDDSService service;
	return &service;
}

bool CSDDSService::Init(const std::string& partition_name) {

	if (!CreateParticipant(partition_name)) {
		return false;
	}

	if (!CreatePublisher()) {
		return false;
	}

	if (!CreateSubscriber()) {
		return false;
	}

	RegisterType();

	if (!CreateTopic("msg")){
		return false;
	}

	if (!CreateWriter()){
		return false;
	}

	if (!CreateReader()){
		return false;
	}

	LogDDSInfo("dds init success")

		return true;
}

bool CSDDSService::CreateParticipant(const std::string&
	participant_name) {

	domain_id_ = DOMAIN_ID_DEFAULT;
	dpf_ = DomainParticipantFactory::get_instance();
	if (!CheckHandle(dpf_.in(), "DDS::DomainParticipantFactory::get_instance")) {
		LogDDSErr("dpf get instance error");
		return false;
	}

	participant_ = dpf_->create_participant(domain_id_, PARTICIPANT_QOS_DEFAULT,
		NULL,
		STATUS_MASK_NONE);
	if (!CheckHandle(participant_.in(),
		"DDS::DomainParticipantFactory::create_participant")) {
		LogDDSErr("create participant error");
		return false;
	}
	partition_name_ = participant_name.c_str();
	LogDDSInfo("create participant success");
	return true;
}

void CSDDSService::DeleteParticipant()
{
	auto status = dpf_->delete_participant(participant_.in());
	if (!CheckStatus(status, "DDS::DomainParticipant::delete_participant ")) {
		LogDDSErr("delete_participant error");
		return;
	}
}

bool CSDDSService::CreateTopic(const std::string& topic_name) {

	TopicQos reliable_topic_qos;
	auto status = participant_->get_default_topic_qos(reliable_topic_qos);
	if (!CheckStatus(status, "DDS::DomainParticipant::get_default_topic_qos")) {
		LogDDSErr("get default topic qos error")
			return false;
	}

	reliable_topic_qos.reliability.kind = RELIABLE_RELIABILITY_QOS;
	reliable_topic_qos.durability.kind = TRANSIENT_DURABILITY_QOS;
	reliable_topic_qos.history.kind = KEEP_ALL_HISTORY_QOS;

	status = participant_->set_default_topic_qos(reliable_topic_qos);
	if (!CheckStatus(status, "DDS::DomainParticipant::set_default_topic_qos")) {
		LogDDSErr("set default topic qos error")
			return false;
	}

	topic_ = participant_->create_topic(topic_name.c_str(),
		type_name_, reliable_topic_qos,
		NULL, STATUS_MASK_NONE);

	if (!CheckHandle(topic_.in(), "DDS::DomainParticipant::create_topic ()")) {
		LogDDSErr("create topic failed, topic name:" + topic_name)
			return false;
	}

	return true;
}

void CSDDSService::RegisterType()
{
	MsgTypeSupport_var mt = new MsgTypeSupport();
	type_name_ = mt->get_type_name();
	auto status = mt->register_type(participant_.in(), type_name_);
	if (!CheckStatus(status, "register_type")) {
		LogDDSErr("register_type error");
	}
}

bool CSDDSService::CreatePublisher() {
	PublisherQos pub_qos;
	auto status = participant_->get_default_publisher_qos(pub_qos);
	if (!CheckStatus(status, "DDS::DomainParticipant::get_default_publisher_qos")) {
		LogDDSErr("get default publisher qos error");
		return false;
	}

	pub_qos.partition.name.length(1);
	pub_qos.partition.name[0] = partition_name_;

	publisher_ = participant_->create_publisher(pub_qos, NULL, STATUS_MASK_NONE);
	if (!CheckHandle(publisher_.in(), "DDS::DomainParticipant::create_publisher")) {
		LogDDSErr("create publisher error");
		return false;
	}
	LogDDSInfo("create publisher success");
	return true;
}

bool CSDDSService::CreateSubscriber() {
	SubscriberQos sub_qos;
	int status = participant_->get_default_subscriber_qos(sub_qos);
	if (!CheckStatus(status,
		"DDS::DomainParticipant::get_default_subscriber_qos")) {
		LogDDSErr("get default subscribe qos error");
		return false;
	}

	sub_qos.partition.name.length(1);
	sub_qos.partition.name[0] = partition_name_;

	subscriber_ = participant_->create_subscriber(sub_qos, NULL, STATUS_MASK_NONE);
	if (!CheckHandle(subscriber_.in(),
		"DDS::DomainParticipant::create_subscriber")) {
		LogDDSErr("create subscribe failed")
			return false;
	}

	return true;
}

bool CSDDSService::CreateWriter() {

	DDS::DataWriterQos dw_qos;
	auto status = publisher_->get_default_datawriter_qos(dw_qos);
	if (!CheckStatus(status, "DDS::DomainParticipant::get_default_publisher_qos")) {
		LogDDSErr("get_default_datawriter_qos failed")
			return false;
	}

	TopicQos reliable_topic_qos;
	status = participant_->get_default_topic_qos(reliable_topic_qos);

	status = publisher_->copy_from_topic_qos(dw_qos, reliable_topic_qos);
	CheckStatus(status, "DDS::Publisher::copy_from_topic_qos");
	dw_qos.writer_data_lifecycle.autodispose_unregistered_instances = true;

	writer_ = publisher_->create_datawriter(topic_.in(), dw_qos,
		NULL,
		STATUS_MASK_NONE);
	if (!CheckHandle(writer_, "DDS::Publisher::create_datawriter")) {
		return false;
	}

	return true;
}

bool CSDDSService::CreateReader() {

	reader_ = subscriber_->create_datareader(topic_.in(),
		DATAREADER_QOS_USE_TOPIC_QOS, NULL, STATUS_MASK_NONE);
	if (!CheckHandle(reader_, "DDS::Subscriber::create_datareader ()")) {
		return false;
	}

	DDS::Property pp;
	pp.name = "ignoreLoansOnDeletion";
	pp.value = "true";
	reader_->set_property(pp);

	newMsg = reader_->create_readcondition(ANY_SAMPLE_STATE,
		ANY_VIEW_STATE, ANY_INSTANCE_STATE);
	if (!CheckHandle(newMsg.in(), "DDS::DataReader::create_readcondition"))
	{
		LogDDSErr("create_readcondition failed");
		return false;
	}

	newMsgWS = new WaitSet();
	auto status = newMsgWS->attach_condition(newMsg.in());     // ReadCondition
	if (!CheckStatus(status, "DDS::WaitSetData::attach_condition (newMsg)")){
		LogDDSErr("attach_condition failed");
		return false;
	}

	return true;
}

bool CSDDSService::write(const MsgData& msg_data){
	Msg msgInstance;
	msgInstance.subjectId = msg_data.subjectId;
	msgInstance.systemId = msg_data.systemId.c_str();
	msgInstance.time = msg_data.time;
	msgInstance.from = msg_data.from.c_str();
	msgInstance.topicName = msg_data.topicName.c_str();
	msgInstance.content = msg_data.content.c_str();

	MsgDataWriter_var HelloWorldWriter = MsgDataWriter::_narrow(writer_.in());

	auto status = HelloWorldWriter->write(msgInstance, DDS::HANDLE_NIL);
	if (!CheckStatus(status, "MsgDataWriter::write")) {
		LogDDSErr("write failed")
			return false;
	}

	return true;
}

std::vector<MsgData> CSDDSService::read(){
	MsgDataReader_var HelloWorldReader = MsgDataReader::_narrow(reader_.in());
	CheckHandle(HelloWorldReader.in(), "MsgDataReader::_narrow");

	MsgSeq msgList;
	SampleInfoSeq infoSeq;
	std::vector<MsgData> dataqueue;

	auto status = HelloWorldReader->read(msgList, infoSeq, LENGTH_UNLIMITED,
		ANY_SAMPLE_STATE, ANY_VIEW_STATE, ANY_INSTANCE_STATE);
	CheckStatus(status, "msgDataReader::take");
	for (DDS::ULong j = 0; j < msgList.length(); j++)
	{
		MsgData data;
		data.subjectId = msgList[j].subjectId;
		data.systemId = msgList[j].systemId;
		data.time = msgList[j].time;
		data.from = msgList[j].from;
		data.topicName = msgList[j].topicName;
		data.content = msgList[j].content;

		dataqueue.push_back(data);
	}

	return dataqueue;
}

std::vector<MsgData> CSDDSService::take(){
	MsgDataReader_var HelloWorldReader = MsgDataReader::_narrow(reader_.in());
	CheckHandle(HelloWorldReader.in(), "MsgDataReader::_narrow");

	MsgSeq msgList;
	SampleInfoSeq infoSeq;
	std::vector<MsgData> dataqueue;

	auto status = HelloWorldReader->take(msgList, infoSeq, LENGTH_UNLIMITED,
		ANY_SAMPLE_STATE, ANY_VIEW_STATE, ANY_INSTANCE_STATE);
	CheckStatus(status, "msgDataReader::take");
	for (DDS::ULong j = 0; j < msgList.length(); j++)
	{
		MsgData data;
		data.subjectId = msgList[j].subjectId;
		data.systemId = msgList[j].systemId;
		data.time = msgList[j].time;
		data.from = msgList[j].from;
		data.topicName = msgList[j].topicName;
		data.content = msgList[j].content;

		dataqueue.push_back(data);
	}

	return dataqueue;
}

void CSDDSService::ReadWithWaitSet(){
	ConditionSeq guardList;
	guardList.length(4);
	MsgSeq msgList;
	SampleInfoSeq infoSeq;

	//std::vector<MsgData> dataqueue;

	wait_timeout.sec = 2;
	wait_timeout.nanosec = 0;

	while (true){
		//dataqueue.clear();
		auto status = newMsgWS->wait(guardList, wait_timeout);
		if (status == DDS::RETCODE_OK) {
			/* Walk over all guards to display information */
			for (DDS::ULong i = 0; i < guardList.length(); i++)
			{
				if (guardList[i].in() == newMsg.in())
				{
					/* The newMsg ReadCondition contains data */
					MsgDataReader_var MsgReader = MsgDataReader::_narrow(reader_.in());
					CheckHandle(MsgReader.in(), "MsgDataReader::_narrow");
					status = MsgReader->take_w_condition(msgList, infoSeq, LENGTH_UNLIMITED,
						newMsg.in());
					CheckStatus(status, "WaitSetData::MsgDataReader::take_w_condition");

					for (DDS::ULong j = 0; j < msgList.length(); j++)
					{
						if (infoSeq[j].valid_data)
						{
							MsgData data;
							data.subjectId = msgList[j].subjectId;
							data.systemId = msgList[j].systemId;
							data.time = msgList[j].time;
							data.from = msgList[j].from;
							data.topicName = msgList[j].topicName;
							data.content = msgList[j].content;

							if (cb_ != nullptr)
							{
								cb_(data);
							}
						}
					}
					

					if (msgList.length() > 0)
					{
						status = MsgReader->return_loan(msgList, infoSeq);
						CheckStatus(status, "WaitSetData::MsgDataReader::return_loan");
					}
				}
			} /* for */
		}
		else if (status != DDS::RETCODE_TIMEOUT) {
			// DDS_RETCODE_TIMEOUT is considered as an error
			// only after it has occurred count times
			CheckStatus(status, "DDS::WaitSetData::wait");
		}
		/*else {
			std::cout << std::endl << "!!! [INFO] WaitSet timedout  "  << std::endl;
		}*/
	}
}

void CSDDSService::StartReceiveData(){
	std::thread th(&CSDDSService::ReadWithWaitSet,this);
	th.detach();
}

void CSDDSService::SetCallBack(std::function<bool(MsgData)> cb){
	cb_ = cb;
}

//void CSDDSService::SetCallBack(void *cb){
//	cb_ = cb;
//}

DataReader_ptr CSDDSService::getReader()
{
	return DataReader::_duplicate(reader_.in());
}

DataWriter_ptr CSDDSService::getWriter()
{
	return DataWriter::_duplicate(writer_.in());
}

Publisher_ptr CSDDSService::getPublisher()
{
	return Publisher::_duplicate(publisher_.in());
}

Subscriber_ptr CSDDSService::getSubscriber()
{
	return Subscriber::_duplicate(subscriber_.in());
}

Topic_ptr CSDDSService::getTopic()
{
	return Topic::_duplicate(topic_.in());
}

DomainParticipant_ptr CSDDSService::getParticipant()
{
	return DomainParticipant::_duplicate(participant_.in());
}
