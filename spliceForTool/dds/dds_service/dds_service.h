﻿#ifndef DDS_SERVICE_H
#define DDS_SERVICE_H

//dds
#include "csdds_service_export.h"
#include "ccpp_dds_dcps.h"

//std
#include <vector>
#include <functional>
#include <map>
#include <thread>
#include <atomic>

using namespace DDS;

typedef struct _MsgData{
	long subjectId;
	std::string systemId;
	double time;
	std::string from;
	std::string topicName;
	std::string content;
}MsgData;

class CSDDS_SERVICE_EXPORT CSDDSService  {
 public:
  static CSDDSService* Instance();

 public:
  bool Init(const std::string& partition_name);

  bool CreateParticipant(const std::string& participant_name) ;
  void DeleteParticipant();

  void RegisterType();
  bool CreateTopic(const std::string& topic_name) ;

  bool CreatePublisher();
  bool CreateSubscriber();
  
  bool CreateWriter(const std::string& topic_name);
  bool CreateReader(const std::string& topic_name);
  
  bool write(const std::string &topic_name, const MsgData& msg_data);
  std::vector<MsgData> read(const std::string &topic_name);
  std::vector<MsgData> take(const std::string &topic_name);

  void ReadWithWaitSet();

  void StartReceiveData();

  void SetCallBack(std::function<bool(MsgData)>);
  //void SetCallBack(void *cb);

  void Clear();

  DataReader_ptr getReader(const std::string& topic_name);
  DataWriter_ptr getWriter(const std::string& topic_name);
  Publisher_ptr getPublisher();
  Subscriber_ptr getSubscriber();
  Topic_ptr getTopic(const std::string &topic_name);
  DomainParticipant_ptr getParticipant();

  CSDDSService();
  ~CSDDSService();
  CSDDSService(const CSDDSService&) = delete;
  CSDDSService& operator=(const CSDDSService&) = delete;

 private:
  DomainParticipantFactory_var dpf_;
  DomainParticipant_var participant_;
  DomainId_t domain_id_;
  DDS::String_var partition_name_;
  DDS::String_var type_name_;

  //Topic_var topic_;

  Publisher_var publisher_;
  Subscriber_var subscriber_;

  //DataWriter_var writer_;
  //DataReader_var reader_;

  //ReadCondition_var newMsg;
  WaitSet_var newMsgWS;
  Duration_t wait_timeout;

  std::function<bool(MsgData)> cb_;
  
  std::thread read_thread_;
  std::atomic<bool> read_flag_ = true;

  std::map<std::string,Topic_var> topics_;
  std::map<std::string, DataReader_var> readers_;
  std::map<std::string, DataWriter_var> writers_;
  std::map<std::string, ReadCondition_var> conditions_;
  //std::map<std::string, WaitSet_var>waitsets_;
  //void *cb_;
};

#endif

