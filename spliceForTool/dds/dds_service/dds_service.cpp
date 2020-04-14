// DDSService.cpp : 定义 DLL 应用程序的导出函数。
//

#include <chrono>
#include <ctime>
#include <thread>

#include "dds_service.h"
#include "check_status.h"

#include "csmongomgr/csmongomgr.h"
#include "cssimconfig/cssimconfig.h"

//msg_type
#include "ccpp_WaitSetData.h"
#include "examples_export.h"

#include "cssimlog/cssimlog.h"

using namespace WaitSetData;

CSDDSService::CSDDSService() {
}

CSDDSService::~CSDDSService() {
	Clear();
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

	/*if (!CreateTopic("msg")){
		return false;
	}

	if (!CreateWriter()){
		return false;
	}

	if (!CreateReader()){
		return false;
	}*/
	newMsgWS = new WaitSet();

	LogDDSInfo("dds init success")

		return true;
}

std::string TimePointToString(std::chrono::time_point<std::chrono::system_clock>
	time_point) {
	std::time_t t = std::chrono::system_clock::to_time_t(time_point);
	std::tm tm2;
	localtime_s(&tm2, &t);
	char buf[100] = { 0 };
	std::strftime(buf, sizeof(buf), "%Y%m%d%H%M%S", &tm2);
	std::string str = buf;
	return std::move(str);
}

bool CSDDSService::InitDataBase() {

	bool rv = false;
	std::string url("mongodb://");
	auto db_config = CSSimConfig::Instance().GetConfig("MongDBConfig");
	if (db_config.size() > 0) {
		auto pip = db_config.find("ip_address");
		if (pip != db_config.end() && !pip->second.empty()) {
			url.append(pip->second);
		}
		else {
			url.append("127.0.0.1");
		}

		url.append(":");

		auto pport = db_config.find("port");
		if (pport != db_config.end() && !pport->second.empty()) {
			url.append(pport->second);
		}
		else {
			url.append("27017");
		}
	}
	else {
		url = "mongodb://localhost:27017";
	}

	rv = Mongo->ConnectDB(url);
	if (!rv) {
		LogEngInfo("connect mongo db failed")
			return rv;
	}

	std::string table_name;
	auto current_time = std::chrono::system_clock::now();
	table_name = scenario_name_ + TimePointToString(std::chrono::system_clock::now());

	rv = Mongo->OpenDB("scenario", table_name);
	can_record_ = rv;

	LogEngInfo("init database success")

	return rv;
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
	read_flag_ = false;
	if (read_thread_.joinable()){
	read_thread_.join();
	}
	
	auto status = dpf_->delete_participant(participant_.in());
	if (!CheckStatus(status, "DDS::DomainParticipant::delete_participant ")) {
		LogDDSErr("delete_participant error");
		return;
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
		LogDDSErr("get default topic qos error")
			return false;
	}

	reliable_topic_qos.reliability.kind = RELIABLE_RELIABILITY_QOS;
	//reliable_topic_qos.durability.kind = TRANSIENT_DURABILITY_QOS;
	reliable_topic_qos.history.kind = KEEP_ALL_HISTORY_QOS;

	status = participant_->set_default_topic_qos(reliable_topic_qos);
	if (!CheckStatus(status, "DDS::DomainParticipant::set_default_topic_qos")) {
		LogDDSErr("set default topic qos error")
			return false;
	}

	Topic_var topic = participant_->create_topic(topic_name.c_str(),
		type_name_, reliable_topic_qos,
		NULL, STATUS_MASK_NONE);

	if (!CheckHandle(topic.in(), "DDS::DomainParticipant::create_topic ()")) {
		LogDDSErr("create topic failed, topic name:" + topic_name)
			return false;
	}

	topics_[topic_name] = topic;
	LogDDSInfo("create topic successed, topic name:" + topic_name)
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

	LogDDSInfo("dds register_type sucess");
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
	LogDDSInfo("create subscribe success");
	return true;
}

bool CSDDSService::CreateWriter(const std::string& topic_name) {

	auto it = writers_.find(topic_name);
	if (it != writers_.end()){
		return true;
	}

	Topic_var topic;
	auto i = topics_.find(topic_name);
	if (i == topics_.end()){
		return false;
	}
	else {
		topic = i->second;
	}

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

	DataWriter_var writer = publisher_->create_datawriter(topic.in(), dw_qos,
		NULL,
		STATUS_MASK_NONE);
	if (!CheckHandle(writer, "DDS::Publisher::create_datawriter")) {
		LogDDSErr("DDS::Publisher::create_datawriter failed")
		return false;
	}

	writers_[topic_name] = writer;
	LogDDSInfo("DDS::Publisher::create_datawriter sucessed")
	return true;
}

bool CSDDSService::CreateReader(const std::string& topic_name) {

	auto it = readers_.find(topic_name);
	if (it != readers_.end()){
		return true;
	}

	Topic_var topic;
	auto i = topics_.find(topic_name);
	if (i == topics_.end()){
		return false;
	}
	else {
		topic = i->second;
	}

	DataReader_var reader = subscriber_->create_datareader(topic.in(),
		DATAREADER_QOS_USE_TOPIC_QOS, NULL, STATUS_MASK_NONE);
	if (!CheckHandle(reader, "DDS::Subscriber::create_datareader ()")) {
		LogDDSErr("create_datareader failed");
		return false;
	}

	DDS::Property pp;
	pp.name = "ignoreLoansOnDeletion";
	pp.value = "true";
	reader->set_property(pp);

	ReadCondition_var newMsg = reader->create_readcondition(ANY_SAMPLE_STATE,
		ANY_VIEW_STATE, ANY_INSTANCE_STATE);
	if (!CheckHandle(newMsg.in(), "DDS::DataReader::create_readcondition"))
	{
		LogDDSErr("create_readcondition failed");
		return false;
	}

	
	auto status = newMsgWS->attach_condition(newMsg.in());     // ReadCondition
	if (!CheckStatus(status, "DDS::WaitSetData::attach_condition (newMsg)")){
		LogDDSErr("attach_condition failed");
		return false;
	}

	conditions_[topic_name] = newMsg;
	readers_[topic_name] = reader;
	LogDDSInfo("create_datareader sucessed");
	return true;
}

bool CSDDSService::write(const std::string &topic_name, const MsgData& msg_data){

	auto it = writers_.find(topic_name);
	if (it == writers_.end()){
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
		LogDDSErr("write failed")
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
	guardList.length(conditions_.size());
	MsgSeq msgList;
	SampleInfoSeq infoSeq;

	//std::vector<MsgData> dataqueue;

	wait_timeout.sec = 2;
	wait_timeout.nanosec = 0;

	while (read_flag_){
		auto status = newMsgWS->wait(guardList, wait_timeout);
		if (status == DDS::RETCODE_OK) {
			std::vector<CSInsertData> topic_docs;
			/* Walk over all guards to display information */
			for (DDS::ULong i = 0; i < guardList.length(); i++)
			{
				for (auto it : conditions_){
					auto topic_name = it.first;
					auto condition = it.second;
				
					if (guardList[i].in() == condition.in())
					{
						/* The newMsg ReadCondition contains data */
						// LogDDSInfo("receive data，topic_name:" + topic_name);
						auto reader = readers_[topic_name];
						MsgDataReader_var MsgReader = MsgDataReader::_narrow(reader.in());
						CheckHandle(MsgReader.in(), "MsgDataReader::_narrow");
						status = MsgReader->take_w_condition(msgList, infoSeq, LENGTH_UNLIMITED,condition.in());
						CheckStatus(status, "WaitSetData::MsgDataReader::take_w_condition");

						std::vector<CSInsertData> samples;

						for (DDS::ULong j = 0; j < msgList.length(); j++)
						{
							if (infoSeq[j].valid_data)
							{
								CSInsertData sam;
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
								
								if (sam.Topic2Doc(data)) {
									samples.push_back(std::move(sam));
								}
								//samples.push_back(sam);
							}
						}

						CSInsertData topic_doc;
						topic_doc.InsertParam(std::move(topic_name), samples);
						topic_docs.push_back(std::move(topic_doc));

						if (msgList.length() > 0)
						{
							status = MsgReader->return_loan(msgList, infoSeq);
							CheckStatus(status, "WaitSetData::MsgDataReader::return_loan");
						}
					}
				}
			} /* for */
			static int time = 0;
			time++;
			CSInsertData time_doc;
			time_doc.InsertBasicParam("time", std::to_string(time));
			time_doc.InsertParam("data", topic_docs);

			Mongo->Insert(time_doc);
		}
		else if (status != DDS::RETCODE_TIMEOUT) {
			// DDS_RETCODE_TIMEOUT is considered as an error
			// only after it has occurred count times
			CheckStatus(status, "DDS::WaitSetData::wait");
		}
		/*else {
		std::cout << std::endl << "!!! [INFO] WaitSet timedout  "  << std::endl;
		}*/
		//std::this_thread::sleep_for(std::chrono::milliseconds(300));
	}
}

void CSDDSService::StartReceiveData(){
	read_flag_ = true;
	read_thread_ = std::thread(&CSDDSService::ReadWithWaitSet, this);
	LogDDSInfo("start read thread successed")
	//read_thread_.detach();
}

void CSDDSService::StopReceiveData(){
	/*停止接收数据线程*/
	if (read_flag_){
		read_flag_ = false;
	}
	LogDDSInfo("stop read thread successed")
}

void CSDDSService::SetCallBack(std::function<bool(MsgData)> cb){
	cb_ = cb;
	LogDDSInfo("set call back successed")
}

void CSDDSService::Clear(){
	read_flag_ = false;

	try
	{
		if (read_thread_.joinable()){
			read_thread_.join();
		}
	}
	catch (const std::exception& e)
	{
		int i = 0;
	}
	read_thread_.~thread();
	LogDDSInfo("stop read thread done")

	ReturnCode_t rv = 0;

	//删除参与者
	if (participant_) {
		rv = participant_->delete_contained_entities();
	}
	LogDDSInfo("open splice delete entities return code: " + std::to_string(rv))
		
	if (dpf_) {
		rv = dpf_->delete_participant(participant_);
	}
	LogDDSInfo("open splice delete part return code: " + std::to_string(rv))
	
	//主题名称，主题
	topics_.clear();
	//主题名称，写者
	writers_.clear();
	//主题名称，读者
	readers_.clear();
	//主题名称，条件
	conditions_.clear();

	//重置数据
	participant_ = nullptr;
	domain_id_ = 0;
	partition_name_ = "";
	type_name_ = "";

	publisher_ = nullptr;
	subscriber_ = nullptr;
	newMsgWS = nullptr;
	memset(&wait_timeout, 0,sizeof(wait_timeout));

	LogDDSInfo("opensplice clear")
}

void CSDDSService::SetScenarioName(const std::string &scenario_name){
	scenario_name_ = scenario_name;
}

DataReader_ptr CSDDSService::getReader(const std::string& topic_name)
{
	auto it = readers_.find(topic_name);
	if (it != readers_.end()){
		return DataReader::_duplicate(it->second.in());
	}
}

DataWriter_ptr CSDDSService::getWriter(const std::string& topic_name)
{
	auto it = writers_.find(topic_name);
	if (it != writers_.end()){
		return DataWriter::_duplicate(it->second.in());
	}
}

Publisher_ptr CSDDSService::getPublisher()
{
	return Publisher::_duplicate(publisher_.in());
}

Subscriber_ptr CSDDSService::getSubscriber()
{
	return Subscriber::_duplicate(subscriber_.in());
}

Topic_ptr CSDDSService::getTopic(const std::string &topic_name)
{
	auto it = topics_.find(topic_name);
	if (it != topics_.end()){
		return Topic::_duplicate(it->second.in());
	}
}

DomainParticipant_ptr CSDDSService::getParticipant()
{
	return DomainParticipant::_duplicate(participant_.in());
}
