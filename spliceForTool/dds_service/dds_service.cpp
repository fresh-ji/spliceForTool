// DDSService.cpp : 定义 DLL 应用程序的导出函数。
//

#include "dds_service.h"

#include "ccpp_WaitSetData.h"
#include "examples_export.h"
#include "check_status.h"

using namespace WaitSetData;

CSDDSService::CSDDSService() {
	threadRun = true;
}

CSDDSService::~CSDDSService() {
	while (1) {
		Sleep(100);
		if (!threadRun) {
			break;
		}
	}
}

CSDDSService* CSDDSService::Instance() {
	try {
		static CSDDSService service;
		return &service;
	}
	catch (DDS::Exception &e) {
		std::string er = e._name();
		LogDDSErr("CSDDSService::Instance " + er);
		return NULL;
	}
	catch (std::exception &e) {
		std::string er = e.what();
		LogDDSErr("CSDDSService::Instance " + er);
		return NULL;
	}
	catch (...) {
		LogDDSCri("CSDDSService::Instance CATCHED!!");
		return NULL;
	}
}

bool CSDDSService::Init(const std::string& partition_name) {
	try {
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
		return true;
	}
	catch (DDS::Exception &e) {
		std::string er = e._name();
		LogDDSErr("CSDDSService::Init " + er);
		return false;
	}
	catch (std::exception &e) {
		std::string er = e.what();
		LogDDSErr("CSDDSService::Init " + er);
		return false;
	}
	catch (...) {
		LogDDSCri("CSDDSService::Init CATCHED!!");
		return false;
	}
}

bool CSDDSService::CreateParticipant(const std::string& participant_name) {
	try {
		domain_id_ = DOMAIN_ID_DEFAULT;

		dpf_ = DomainParticipantFactory::get_instance();
		if (!CheckHandle(dpf_.in(), "DDS::DomainParticipantFactory::get_instance")) {
			return false;
		}

		participant_ = dpf_->create_participant(domain_id_,
			PARTICIPANT_QOS_DEFAULT, NULL, STATUS_MASK_NONE);
		if (!CheckHandle(participant_.in(), "DDS::DomainParticipantFactory::create_participant")) {
			return false;
		}

		partition_name_ = participant_name.c_str();
		return true;
	}
	catch (DDS::Exception &e) {
		std::string er = e._name();
		LogDDSErr("CSDDSService::CreateParticipant " + er);
		return false;
	}
	catch (std::exception &e) {
		std::string er = e.what();
		LogDDSErr("CSDDSService::CreateParticipant " + er);
		return false;
	}
	catch (...) {
		LogDDSCri("CSDDSService::CreateParticipant CATCHED!!");
		return false;
	}
}

bool CSDDSService::CreatePublisher() {
	try {
		PublisherQos pub_qos;

		auto status = participant_->get_default_publisher_qos(pub_qos);
		if (!CheckStatus(status, "DDS::DomainParticipant::get_default_publisher_qos")) {
			return false;
		}

		pub_qos.partition.name.length(1);
		pub_qos.partition.name[0] = partition_name_;

		publisher_ = participant_->create_publisher(pub_qos, NULL, STATUS_MASK_NONE);
		if (!CheckHandle(publisher_.in(), "DDS::DomainParticipant::create_publisher")) {
			return false;
		}
		return true;
	}
	catch (DDS::Exception &e) {
		std::string er = e._name();
		LogDDSErr("CSDDSService::CreatePublisher " + er);
		return false;
	}
	catch (std::exception &e) {
		std::string er = e.what();
		LogDDSErr("CSDDSService::CreatePublisher " + er);
		return false;
	}
	catch (...) {
		LogDDSCri("CSDDSService::CreatePublisher CATCHED!!");
		return false;
	}
}

bool CSDDSService::CreateSubscriber() {
	try {
		SubscriberQos sub_qos;

		int status = participant_->get_default_subscriber_qos(sub_qos);
		if (!CheckStatus(status, "DDS::DomainParticipant::get_default_subscriber_qos")) {
			return false;
		}

		sub_qos.partition.name.length(1);
		sub_qos.partition.name[0] = partition_name_;

		subscriber_ = participant_->create_subscriber(sub_qos, NULL, STATUS_MASK_NONE);
		if (!CheckHandle(subscriber_.in(), "DDS::DomainParticipant::create_subscriber")) {
			return false;
		}
		return true;
	}
	catch (DDS::Exception &e) {
		std::string er = e._name();
		LogDDSErr("CSDDSService::CreateSubscriber " + er);
		return false;
	}
	catch (std::exception &e) {
		std::string er = e.what();
		LogDDSErr("CSDDSService::CreateSubscriber " + er);
		return false;
	}
	catch (...) {
		LogDDSCri("CSDDSService::CreateSubscriber CATCHED!!");
		return false;
	}
}

void CSDDSService::RegisterType() {
	try {
		MsgTypeSupport_var mt = new MsgTypeSupport();
		type_name_ = mt->get_type_name();

		auto status = mt->register_type(participant_.in(), type_name_);
		CheckStatus(status, "register_type");
	}
	catch (DDS::Exception &e) {
		std::string er = e._name();
		LogDDSErr("CSDDSService::RegisterType " + er);
	}
	catch (std::exception &e) {
		std::string er = e.what();
		LogDDSErr("CSDDSService::RegisterType " + er);
	}
	catch (...) {
		LogDDSCri("CSDDSService::RegisterType CATCHED!!");
	}
}

bool CSDDSService::CreateTopic(const std::string& topic_name) {
	try {
		auto it = topics_.find(topic_name);
		if (it != topics_.end()){
			return true;
		}

		TopicQos reliable_topic_qos;
		auto status = participant_->get_default_topic_qos(reliable_topic_qos);
		if (!CheckStatus(status, "DDS::DomainParticipant::get_default_topic_qos")) {
			return false;
		}

		reliable_topic_qos.reliability.kind = RELIABLE_RELIABILITY_QOS;
		// reliable_topic_qos.durability.kind = TRANSIENT_DURABILITY_QOS;
		reliable_topic_qos.history.kind = KEEP_ALL_HISTORY_QOS;

		status = participant_->set_default_topic_qos(reliable_topic_qos);
		if (!CheckStatus(status, "DDS::DomainParticipant::set_default_topic_qos")) {
			return false;
		}

		Topic_var topic = participant_->create_topic(topic_name.c_str(),
			type_name_, reliable_topic_qos, NULL, STATUS_MASK_NONE);

		if (!CheckHandle(topic.in(), "DDS::DomainParticipant::create_topic")) {
			return false;
		}

		topics_[topic_name] = topic;
		return true;

	}
	catch (DDS::Exception &e) {
		std::string er = e._name();
		LogDDSErr("CSDDSService::CreateTopic " + er);
		return false;
	}
	catch (std::exception &e) {
		std::string er = e.what();
		LogDDSErr("CSDDSService::CreateTopic " + er);
		return false;
	}
	catch (...) {
		LogDDSCri("CSDDSService::CreateTopic CATCHED!!");
		return false;
	}
}

bool CSDDSService::CreateWriter(const std::string& topic_name) {
	try {
		auto it = writers_.find(topic_name);
		if (it != writers_.end()) {
			return true;
		}

		Topic_var topic;
		auto i = topics_.find(topic_name);
		if (i == topics_.end()) {
			return false;
		}
		topic = i->second;

		DDS::DataWriterQos dw_qos;
		auto status = publisher_->get_default_datawriter_qos(dw_qos);
		if (!CheckStatus(status,
			"DDS::DomainParticipant::get_default_datawriter_qos")) {
			return false;
		}

		TopicQos reliable_topic_qos;
		status = participant_->get_default_topic_qos(reliable_topic_qos);
		if (!CheckStatus(status, "DDS::DomainParticipant::get_default_topic_qos")) {
			return false;
		}

		status = publisher_->copy_from_topic_qos(dw_qos, reliable_topic_qos);
		CheckStatus(status, "DDS::Publisher::copy_from_topic_qos");
		dw_qos.writer_data_lifecycle.autodispose_unregistered_instances = true;

		DataWriter_var writer = publisher_->create_datawriter(
			topic.in(), dw_qos, NULL, STATUS_MASK_NONE);
		if (!CheckHandle(writer, "DDS::Publisher::create_datawriter")) {
			return false;
		}

		writers_[topic_name] = writer;
		return true;
	}
	catch (DDS::Exception &e) {
		std::string er = e._name();
		LogDDSErr("CSDDSService::CreateWriter " + er);
		return false;
	}
	catch (std::exception &e) {
		std::string er = e.what();
		LogDDSErr("CSDDSService::CreateWriter " + er);
		return false;
	}
	catch (...) {
		LogDDSCri("CSDDSService::CreateWriter CATCHED!!");
		return false;
	}
}

bool CSDDSService::CreateReader(const std::string& topic_name) {
	try {
		auto it = readers_.find(topic_name);
		if (it != readers_.end()) {
			return true;
		}

		Topic_var topic;
		auto i = topics_.find(topic_name);
		if (i == topics_.end()) {
			return false;
		}
		topic = i->second;

		DDS::DataReaderQos dr_qos;
		auto status = subscriber_->get_default_datareader_qos(dr_qos);
		if (!CheckStatus(status,
			"DDS::DomainParticipant::get_default_datareader_qos")) {
			return false;
		}

		TopicQos reliable_topic_qos;
		status = participant_->get_default_topic_qos(reliable_topic_qos);
		if (!CheckStatus(status, "DDS::DomainParticipant::get_default_topic_qos")) {
			return false;
		}

		status = subscriber_->copy_from_topic_qos(dr_qos, reliable_topic_qos);
		CheckStatus(status, "DDS::Subscriber::copy_from_topic_qos");
		dr_qos.reader_data_lifecycle.autopurge_disposed_samples_delay = DDS::DURATION_ZERO;

		DataReader_var reader = subscriber_->create_datareader(
			topic.in(), dr_qos, NULL, STATUS_MASK_NONE);
		if (!CheckHandle(reader, "DDS::Subscriber::create_datareader")) {
			return false;
		}

		DDS::Property pp;
		pp.name = "ignoreLoansOnDeletion";
		pp.value = "true";
		reader->set_property(pp);

		ReadCondition_var newMsg = reader->create_readcondition(
			ANY_SAMPLE_STATE, ANY_VIEW_STATE, ANY_INSTANCE_STATE);
		if (!CheckHandle(newMsg.in(), "DDS::DataReader::create_readcondition")) {
			return false;
		}

		status = newMsgWS->attach_condition(newMsg.in());
		if (!CheckStatus(status, "DDS::WaitSetData::attach_condition")){
			return false;
		}

		conditions_[topic_name] = newMsg;
		readers_[topic_name] = reader;
		return true;
	}
	catch (DDS::Exception &e) {
		std::string er = e._name();
		LogDDSErr("CSDDSService::CreateReader " + er);
		return false;
	}
	catch (std::exception &e) {
		std::string er = e.what();
		LogDDSErr("CSDDSService::CreateReader " + er);
		return false;
	}
	catch (...) {
		LogDDSCri("CSDDSService::CreateReader CATCHED!!");
		return false;
	}
}

bool CSDDSService::write(const std::string &topic_name, const MsgData& msg_data) {
	try {
		auto it = writers_.find(topic_name);
		if (it == writers_.end()) {
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
			return false;
		}
		return true;
	}
	catch (DDS::Exception &e) {
		std::string er = e._name();
		LogDDSErr("CSDDSService::write " + er);
		return false;
	}
	catch (std::exception &e) {
		std::string er = e.what();
		LogDDSErr("CSDDSService::write " + er);
		return false;
	}
	catch (...) {
		LogDDSCri("CSDDSService::write CATCHED!!");
		return false;
	}
}

std::vector<MsgData> CSDDSService::read(const std::string &topic_name) {

	std::vector<MsgData> dataqueue;
	try {
		auto it = readers_.find(topic_name);
		DataReader_var reader = it->second;

		MsgDataReader_var HelloWorldReader = MsgDataReader::_narrow(reader.in());
		if (!CheckHandle(HelloWorldReader.in(), "MsgDataReader::_narrow")) {
			return dataqueue;
		}

		MsgSeq msgList;
		SampleInfoSeq infoSeq;

		auto status = HelloWorldReader->read(msgList, infoSeq, LENGTH_UNLIMITED,
			ANY_SAMPLE_STATE, ANY_VIEW_STATE, ANY_INSTANCE_STATE);
		if (!CheckStatus(status, "msgDataReader::read")) {
			return dataqueue;
		}
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
	catch (DDS::Exception &e) {
		std::string er = e._name();
		LogDDSErr("CSDDSService::read " + er);
		return dataqueue;
	}
	catch (std::exception &e) {
		std::string er = e.what();
		LogDDSErr("CSDDSService::read " + er);
		return dataqueue;
	}
	catch (...) {
		LogDDSCri("CSDDSService::read CATCHED!!");
		return dataqueue;
	}
}

std::vector<MsgData> CSDDSService::take(const std::string &topic_name) {

	std::vector<MsgData> dataqueue;
	try {
		auto it = readers_.find(topic_name);
		DataReader_var reader = it->second;

		MsgDataReader_var HelloWorldReader = MsgDataReader::_narrow(reader.in());
		if (!CheckHandle(HelloWorldReader.in(), "MsgDataReader::_narrow")) {
			return dataqueue;
		}

		MsgSeq msgList;
		SampleInfoSeq infoSeq;

		auto status = HelloWorldReader->take(msgList, infoSeq, LENGTH_UNLIMITED,
			ANY_SAMPLE_STATE, ANY_VIEW_STATE, ANY_INSTANCE_STATE);
		if (!CheckStatus(status, "msgDataReader::take")) {
			return dataqueue;
		}
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
	catch (DDS::Exception &e) {
		std::string er = e._name();
		LogDDSErr("CSDDSService::take " + er);
		return dataqueue;
	}
	catch (std::exception &e) {
		std::string er = e.what();
		LogDDSErr("CSDDSService::take " + er);
		return dataqueue;
	}
	catch (...) {
		LogDDSCri("CSDDSService::take CATCHED!!");
		return dataqueue;
	}
}

void CSDDSService::ReadWithWaitSet() {
	try {
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
							auto reader = readers_[topic_name];
							MsgDataReader_var MsgReader = MsgDataReader::_narrow(reader.in());
							if (!CheckHandle(MsgReader.in(), "MsgDataReader::_narrow")) {
								continue;
							}
							status = MsgReader->take_w_condition(
								msgList, infoSeq, LENGTH_UNLIMITED, condition.in());
							if (!CheckStatus(
								status, "WaitSetData::MsgDataReader::take_w_condition")) {
								continue;
							}

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
								if (!CheckStatus(
									status, "WaitSetData::MsgDataReader::return_loan")) {
									continue;
								}
							}
						}
					}
				}
			}
			else if (status != DDS::RETCODE_TIMEOUT) {
				// DDS_RETCODE_TIMEOUT is considered as an error
				// only after it has occurred count times
				if (!CheckStatus(status, "DDS::WaitSetData::wait")) {
					continue;
				}
			}
			else {
				// LogDDSErr("CSDDSService::ReadWithWaitSet 1 WaitSet timeout");
			}
		}
		LogDDSInfo("CSDDSService::ReadWithWaitSet thread ends here");
	}
	catch (DDS::Exception &e) {
		std::string er = e._name();
		LogDDSErr("CSDDSService::ReadWithWaitSet 2 " + er);
	}
	catch (std::exception &e) {
		std::string er = e.what();
		LogDDSErr("CSDDSService::ReadWithWaitSet 3 " + er);
	}
	catch (...) {
		LogDDSCri("CSDDSService::ReadWithWaitSet CATCHED!!");
	}
}

void CSDDSService::StartReceiveData() {
	try {
		read_flag_ = true;
		read_thread_ = std::thread(&CSDDSService::ReadWithWaitSet, this);
		read_thread_.detach();
	}
	catch (DDS::Exception &e) {
		std::string er = e._name();
		LogDDSErr("CSDDSService::StartReceiveData " + er);
	}
	catch (std::exception &e) {
		std::string er = e.what();
		LogDDSErr("CSDDSService::StartReceiveData " + er);
	}
	catch (...) {
		LogDDSCri("CSDDSService::StartReceiveData CATCHED!!");
	}
}

void CSDDSService::StopReceiveData() {
	try {
		if (read_flag_) {
			read_flag_ = false;
		}
		Clear();
	}
	catch (DDS::Exception &e) {
		std::string er = e._name();
		LogDDSErr("CSDDSService::StopReceiveData " + er);
	}
	catch (std::exception &e) {
		std::string er = e.what();
		LogDDSErr("CSDDSService::StopReceiveData " + er);
	}
	catch (...) {
		LogDDSCri("CSDDSService::StopReceiveData CATCHED!!");
	}
}

void CSDDSService::SetCallBack(std::function<bool(MsgData)> cb) {
	cb_ = cb;
}

void CSDDSService::Clear() {
	try{
		//if (read_thread_.joinable()){
		//	read_thread_.join();
		//}

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

		threadRun = false;
	}
	catch (DDS::Exception &e) {
		std::string er = e._name();
		LogDDSErr("CSDDSService::Clear " + er);
	}
	catch (std::exception &e) {
		std::string er = e.what();
		LogDDSErr("CSDDSService::Clear " + er);
	}
	catch (...) {
		LogDDSCri("CSDDSService::Clear CATCHED!!");
	}
}

DataReader_ptr CSDDSService::getReader(const std::string& topic_name) {
	auto it = readers_.find(topic_name);
	if (it != readers_.end()){
		return DataReader::_duplicate(it->second.in());
	}
	return NULL;
}

DataWriter_ptr CSDDSService::getWriter(const std::string& topic_name) {
	auto it = writers_.find(topic_name);
	if (it != writers_.end()){
		return DataWriter::_duplicate(it->second.in());
	}
	return NULL;
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
	return NULL;
}

DomainParticipant_ptr CSDDSService::getParticipant() {
	return DomainParticipant::_duplicate(participant_.in());
}
