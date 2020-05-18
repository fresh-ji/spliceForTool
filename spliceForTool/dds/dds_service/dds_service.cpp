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
	try {
		static CSDDSService service;
		return &service;
	}
	catch (...) {
		std::cout << "[CATCHED] CSDDSService::Instance" << std::endl;
	}
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

	newMsgWS = new WaitSet();
	logMe(1, "dds init success");
	return true;
}

bool CSDDSService::CreateParticipant(const std::string& participant_name) {

	domain_id_ = DOMAIN_ID_DEFAULT;

	dpf_ = DomainParticipantFactory::get_instance();
	if (!CheckHandle(dpf_.in(), "DDS::DomainParticipantFactory::get_instance")) {
		logMe(2, "DDS::DomainParticipantFactory::get_instance");
		return false;
	}

	participant_ = dpf_->create_participant(domain_id_,
		PARTICIPANT_QOS_DEFAULT, NULL, STATUS_MASK_NONE);
	if (!CheckHandle(participant_.in(), "DDS::DomainParticipantFactory::create_participant")) {
		logMe(2, "DDS::DomainParticipantFactory::create_participant");
		return false;
	}

	partition_name_ = participant_name.c_str();
	logMe(1, "create participant success");
	return true;
}

bool CSDDSService::CreatePublisher() {

	PublisherQos pub_qos;

	auto status = participant_->get_default_publisher_qos(pub_qos);
	if (!CheckStatus(status, "DDS::DomainParticipant::get_default_publisher_qos")) {
		logMe(2, "DDS::DomainParticipant::get_default_publisher_qos");
		return false;
	}

	pub_qos.partition.name.length(1);
	pub_qos.partition.name[0] = partition_name_;

	publisher_ = participant_->create_publisher(pub_qos, NULL, STATUS_MASK_NONE);
	if (!CheckHandle(publisher_.in(), "DDS::DomainParticipant::create_publisher")) {
		logMe(2, "DDS::DomainParticipant::create_publisher");
		return false;
	}
	logMe(1, "create publisher success");
	return true;
}

bool CSDDSService::CreateSubscriber() {

	SubscriberQos sub_qos;

	int status = participant_->get_default_subscriber_qos(sub_qos);
	if (!CheckStatus(status, "DDS::DomainParticipant::get_default_subscriber_qos")) {
		logMe(2, "DDS::DomainParticipant::get_default_subscriber_qos");
		return false;
	}

	sub_qos.partition.name.length(1);
	sub_qos.partition.name[0] = partition_name_;

	subscriber_ = participant_->create_subscriber(sub_qos, NULL, STATUS_MASK_NONE);
	if (!CheckHandle(subscriber_.in(), "DDS::DomainParticipant::create_subscriber")) {
		logMe(2, "DDS::DomainParticipant::create_subscriber");
		return false;
	}
	logMe(1, "create subscriber success");
	return true;
}

void CSDDSService::RegisterType() {

	MsgTypeSupport_var mt = new MsgTypeSupport();
	type_name_ = mt->get_type_name();

	auto status = mt->register_type(participant_.in(), type_name_);
	if (!CheckStatus(status, "register_type")) {
		logMe(2, "register_type");
	}

	logMe(1, "register_type success");
}

void CSDDSService::DeleteParticipant() {

	read_flag_ = false;
	if (read_thread_.joinable()){
		read_thread_.join();
	}

	auto status = dpf_->delete_participant(participant_.in());
	if (!CheckStatus(status, "DDS::DomainParticipant::delete_participant")) {
		logMe(2, "DDS::DomainParticipant::delete_participant");
	}
}

bool CSDDSService::CreateTopic(const std::string& topic_name) {

	auto it = topics_.find(topic_name);
	if (it != topics_.end()){
		return true;
	}

	TopicQos reliable_topic_qos;
	auto status = participant_->get_default_topic_qos(reliable_topic_qos);
	if (!CheckStatus(status, "DDS::DomainParticipant::get_default_topic_qos")) {
		logMe(2, "DDS::DomainParticipant::get_default_topic_qos");
		return false;
	}

	reliable_topic_qos.reliability.kind = RELIABLE_RELIABILITY_QOS;
	// reliable_topic_qos.durability.kind = TRANSIENT_DURABILITY_QOS;
	reliable_topic_qos.history.kind = KEEP_ALL_HISTORY_QOS;

	status = participant_->set_default_topic_qos(reliable_topic_qos);
	if (!CheckStatus(status, "DDS::DomainParticipant::set_default_topic_qos")) {
		logMe(2, "DDS::DomainParticipant::set_default_topic_qos");
		return false;
	}

	Topic_var topic = participant_->create_topic(topic_name.c_str(),
		type_name_, reliable_topic_qos, NULL, STATUS_MASK_NONE);

	if (!CheckHandle(topic.in(), "DDS::DomainParticipant::create_topic")) {
		logMe(2, "DDS::DomainParticipant::create_topic");
		return false;
	}

	topics_[topic_name] = topic;
	logMe(1, "create topic successed, topic name : " + topic_name);
	return true;
}

bool CSDDSService::CreateWriter(const std::string& topic_name) {

	auto it = writers_.find(topic_name);
	if (it != writers_.end()) {
		logMe(2, "CreateWriter already exist");
		return true;
	}

	Topic_var topic;
	auto i = topics_.find(topic_name);
	if (i == topics_.end()) {
		logMe(2, "CreateWriter no topic");
		return false;
	}
	topic = i->second;

	DDS::DataWriterQos dw_qos;
	auto status = publisher_->get_default_datawriter_qos(dw_qos);
	if (!CheckStatus(status, "DDS::DomainParticipant::get_default_publisher_qos")) {
		logMe(2, "DDS::DomainParticipant::get_default_publisher_qos");
		return false;
	}

	TopicQos reliable_topic_qos;
	status = participant_->get_default_topic_qos(reliable_topic_qos);

	status = publisher_->copy_from_topic_qos(dw_qos, reliable_topic_qos);
	CheckStatus(status, "DDS::Publisher::copy_from_topic_qos");
	dw_qos.writer_data_lifecycle.autodispose_unregistered_instances = true;

	DataWriter_var writer = publisher_->create_datawriter(
		topic.in(), dw_qos, NULL, STATUS_MASK_NONE);
	if (!CheckHandle(writer, "DDS::Publisher::create_datawriter")) {
		logMe(2, "DDS::Publisher::create_datawriter");
		return false;
	}

	writers_[topic_name] = writer;
	logMe(1, "create_datawriter successed");
	return true;
}

bool CSDDSService::CreateReader(const std::string& topic_name) {

	auto it = readers_.find(topic_name);
	if (it != readers_.end()) {
		logMe(2, "CreateReader already exist");
		return true;
	}

	Topic_var topic;
	auto i = topics_.find(topic_name);
	if (i == topics_.end()) {
		logMe(2, "CreateReader no topic");
		return false;
	}
	topic = i->second;

	DataReader_var reader = subscriber_->create_datareader(topic.in(),
		DATAREADER_QOS_USE_TOPIC_QOS, NULL, STATUS_MASK_NONE);
	if (!CheckHandle(reader, "DDS::Subscriber::create_datareader")) {
		logMe(2, "DDS::Subscriber::create_datareader");
		return false;
	}

	DDS::Property pp;
	pp.name = "ignoreLoansOnDeletion";
	pp.value = "true";
	reader->set_property(pp);

	ReadCondition_var newMsg = reader->create_readcondition(
		ANY_SAMPLE_STATE, ANY_VIEW_STATE, ANY_INSTANCE_STATE);
	if (!CheckHandle(newMsg.in(), "DDS::DataReader::create_readcondition")) {
		logMe(2, "DDS::DataReader::create_readcondition");
		return false;
	}

	auto status = newMsgWS->attach_condition(newMsg.in());
	if (!CheckStatus(status, "DDS::WaitSetData::attach_condition")){
		logMe(2, "DDS::WaitSetData::attach_condition");
		return false;
	}

	conditions_[topic_name] = newMsg;
	readers_[topic_name] = reader;
	logMe(1, "create_datareader successed");
	return true;
}

bool CSDDSService::write(const std::string &topic_name, const MsgData& msg_data) {

	auto it = writers_.find(topic_name);
	if (it == writers_.end()) {
		logMe(2, "write no topic");
		return false;
	}

	DataWriter_var writer = it->second;

	Msg msgInstance;
	msgInstance.subjectId = msg_data.subjectId;
	msgInstance.systemId = msg_data.systemId.c_str();
	msgInstance.time = msg_data.time;
	msgInstance.from = msg_data.from.c_str();
	msgInstance.topicName = msg_data.topicName.c_str();
	msgInstance.content = msg_data.content.c_str();

	MsgDataWriter_var HelloWorldWriter = MsgDataWriter::_narrow(writer.in());

	auto status = HelloWorldWriter->write(msgInstance, DDS::HANDLE_NIL);
	if (!CheckStatus(status, "MsgDataWriter::write")) {
		logMe(2, "MsgDataWriter::write");
		return false;
	}

	return true;
}

std::vector<MsgData> CSDDSService::read(const std::string &topic_name){

	auto it = readers_.find(topic_name);
	DataReader_var reader = it->second;

	MsgDataReader_var HelloWorldReader = MsgDataReader::_narrow(reader.in());
	CheckHandle(HelloWorldReader.in(), "MsgDataReader::_narrow");

	MsgSeq msgList;
	SampleInfoSeq infoSeq;
	std::vector<MsgData> dataqueue;

	auto status = HelloWorldReader->read(msgList, infoSeq, LENGTH_UNLIMITED,
		ANY_SAMPLE_STATE, ANY_VIEW_STATE, ANY_INSTANCE_STATE);
	CheckStatus(status, "msgDataReader::take");
	for (DDS::ULong j = 0; j < msgList.length(); j++) {
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

std::vector<MsgData> CSDDSService::take(const std::string &topic_name){

	auto it = readers_.find(topic_name);
	DataReader_var reader = it->second;

	MsgDataReader_var HelloWorldReader = MsgDataReader::_narrow(reader.in());
	CheckHandle(HelloWorldReader.in(), "MsgDataReader::_narrow");

	MsgSeq msgList;
	SampleInfoSeq infoSeq;
	std::vector<MsgData> dataqueue;

	auto status = HelloWorldReader->take(msgList, infoSeq, LENGTH_UNLIMITED,
		ANY_SAMPLE_STATE, ANY_VIEW_STATE, ANY_INSTANCE_STATE);
	CheckStatus(status, "msgDataReader::take");
	for (DDS::ULong j = 0; j < msgList.length(); j++) {
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
	guardList.length(conditions_.size());
	MsgSeq msgList;
	SampleInfoSeq infoSeq;

	wait_timeout.sec = 2;
	wait_timeout.nanosec = 0;

	while (read_flag_) {
		auto status = newMsgWS->wait(guardList, wait_timeout);
		if (status == DDS::RETCODE_OK) {
			/* Walk over all guards to display information */
			for (DDS::ULong i = 0; i < guardList.length(); i++) {
				for (auto it : conditions_) {
					auto topic_name = it.first;
					auto condition = it.second;

					if (guardList[i].in() == condition.in()) {
						/* The newMsg ReadCondition contains data */
						// LogDDSInfo("receive data，topic_name:" + topic_name);
						auto reader = readers_[topic_name];
						MsgDataReader_var MsgReader = MsgDataReader::_narrow(reader.in());
						CheckHandle(MsgReader.in(), "MsgDataReader::_narrow");
						status = MsgReader->take_w_condition(msgList, infoSeq, LENGTH_UNLIMITED, condition.in());
						CheckStatus(status, "WaitSetData::MsgDataReader::take_w_condition");

						for (DDS::ULong j = 0; j < msgList.length(); j++) {
							if (infoSeq[j].valid_data) {
								MsgData data;
								data.subjectId = msgList[j].subjectId;
								data.systemId = msgList[j].systemId;
								data.time = msgList[j].time;
								data.from = msgList[j].from;
								data.topicName = msgList[j].topicName;
								data.content = msgList[j].content;

								if (cb_ != nullptr) {
									cb_(data);
								}
							}
						}
						if (msgList.length() > 0) {
							status = MsgReader->return_loan(msgList, infoSeq);
							CheckStatus(status, "WaitSetData::MsgDataReader::return_loan");
						}
					}
				}
			} /* for */
		}
		else if (status != DDS::RETCODE_TIMEOUT) {
			// DDS_RETCODE_TIMEOUT is considered as an error
			// only after it has occurred count times
			CheckStatus(status, "DDS::WaitSetData::wait");
		}
		/*
		else {
		std::cout << std::endl << "!!! [INFO] WaitSet timedout  "  << std::endl;
		}
		*/
		// std::this_thread::sleep_for(std::chrono::milliseconds(300));
	}
}

void CSDDSService::StartReceiveData() {
	read_flag_ = true;
	read_thread_ = std::thread(&CSDDSService::ReadWithWaitSet, this);
	logMe(1, "start read thread successed");
	read_thread_.detach();
}

void CSDDSService::StopReceiveData(){
	if (read_flag_) {
		read_flag_ = false;
	}
	logMe(1, "stop read thread successed");
	Clear();
}

void CSDDSService::SetCallBack(std::function<bool(MsgData)> cb){
	cb_ = cb;
}

void CSDDSService::Clear() {

	read_flag_ = false;

	try {
		if (read_thread_.joinable()){
			read_thread_.join();
		}
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	logMe(1, "stop read thread done");

	ReturnCode_t rv = 0;

	if (participant_) {
		rv = participant_->delete_contained_entities();
	}

	if (dpf_) {
		rv = dpf_->delete_participant(participant_);
	}

	topics_.clear();
	writers_.clear();
	readers_.clear();
	conditions_.clear();

	participant_ = nullptr;
	domain_id_ = 0;
	partition_name_ = "";
	type_name_ = "";

	publisher_ = nullptr;
	subscriber_ = nullptr;
	newMsgWS = nullptr;
	memset(&wait_timeout, 0, sizeof(wait_timeout));

	logMe(1, "opensplice clear");
}

DataReader_ptr CSDDSService::getReader(const std::string& topic_name) {
	auto it = readers_.find(topic_name);
	if (it != readers_.end()){
		return DataReader::_duplicate(it->second.in());
	}
}

DataWriter_ptr CSDDSService::getWriter(const std::string& topic_name) {
	auto it = writers_.find(topic_name);
	if (it != writers_.end()){
		return DataWriter::_duplicate(it->second.in());
	}
}

Publisher_ptr CSDDSService::getPublisher() {
	return Publisher::_duplicate(publisher_.in());
}

Subscriber_ptr CSDDSService::getSubscriber() {
	return Subscriber::_duplicate(subscriber_.in());
}

Topic_ptr CSDDSService::getTopic(const std::string &topic_name) {
	auto it = topics_.find(topic_name);
	if (it != topics_.end()){
		return Topic::_duplicate(it->second.in());
	}
}

DomainParticipant_ptr CSDDSService::getParticipant() {
	return DomainParticipant::_duplicate(participant_.in());
}

void CSDDSService::logMe(int type, std::string msg) {

	if (1 == type) {
		// LogDDSInfo(msg);
	}
	else if (2 == type) {
		// LogDDSErr(msg);
	}
}
