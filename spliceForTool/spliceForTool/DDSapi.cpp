
#include "stdafx.h"
#include "DDSapi.h"

class ReadCondHandler {
public:
	/**
	* @param dataState The dataState on which to filter the samples
	*/
	ReadCondHandler(dds::sub::status::DataState& dataState, Interface *i)
		: dataState(dataState) {
		this->i = i;
	}
	void operator() (const dds::sub::cond::ReadCondition& cond) {
		/** retrieve the DataState from the condition */
		dds::sub::status::DataState dataState = cond.state_filter();
		/** retrieve the associated reader from the condition */
		dds::sub::DataReader<Msg> dr = cond.data_reader();

		dds::sub::LoanedSamples<Msg> samples = dr.select().state(dataState).take();
		// dds::sub::LoanedSamples<Msg> samples = dr.select().content(cond).take();
		// dds::sub::LoanedSamples<Msg> samples = dr.take();

		for (dds::sub::LoanedSamples<Msg>::const_iterator sample = samples.begin();
			sample < samples.end(); ++sample) {
			if ((*sample).info().valid()) {
				i->process(sample->data());
			}
		}
	}
private:
	Interface *i;
	dds::sub::status::DataState& dataState;
};

DDSapi::DDSapi(Interface* i) {
	this->i = i;
}

bool DDSapi::startServerDDS() {

	string msg;

	try{
		dds::domain::DomainParticipant dp(i->systemRunId);

		dds::pub::qos::PublisherQos pubQos
			= dp.default_publisher_qos()
			<< dds::core::policy::Partition("WaitSet example");
		dds::pub::Publisher pub(dp, pubQos);

		dds::sub::qos::SubscriberQos subQos
			= dp.default_subscriber_qos()
			<< dds::core::policy::Partition("WaitSet example");
		dds::sub::Subscriber sub(dp, subQos);

		dds::topic::qos::TopicQos topicQos = dp.default_topic_qos()
			<< dds::core::policy::History::KeepAll();

		for (auto n : i->pubNames) {
			dds::topic::Topic<Msg> topic(dp, (const string &)n, topicQos);
			dds::pub::qos::DataWriterQos dwqos = topic.qos();
			dwqos << dds::core::policy::WriterDataLifecycle
				::AutoDisposeUnregisteredInstances();
			dds::pub::DataWriter<Msg> dw(pub, topic, dwqos);
			writers.insert(make_pair(n, dw));
		}

		for (auto n : i->subNames) {
			dds::topic::Topic<Msg> topic(dp, (const string &)n, topicQos);
			dds::sub::qos::DataReaderQos drqos = topic.qos();
			dds::sub::DataReader<Msg> dr(sub, topic, drqos);
			readers.insert(make_pair(n, dr));

			dds::sub::status::DataState *newDataState
				= new dds::sub::status::DataState();
			(*newDataState) << dds::sub::status::SampleState::not_read()
				<< dds::sub::status::InstanceState::any();
			ReadCondHandler *readCondHandler =
				new ReadCondHandler(*newDataState, i);
			dds::sub::cond::ReadCondition readCond(
				dr, *newDataState, *readCondHandler);

			waitSet += readCond;
		}
		return true;
	}
	catch (std::runtime_error& e) {
		msg = i->systemId + " runtime " + e.what();
		// TODO LogDDSErr(msg);
		return false;
	}
	catch (exception &e) {
		msg = i->systemId + " exception " + e.what();
		// TODO LogDDSErr(msg);
		return false;
	}
}

bool DDSapi::publish(string topic, string data) {
	//random_device rd;
	//mt19937 mt(rd());

	Msg message;
	//message.subjectId() = mt();
	message.systemId() = i->systemId;
	message.from() = i->nodeName;
	message.time() = i->currentTime;
	message.topicName() = topic;
	message.content() = data;

	dds::pub::DataWriter<Msg> writer = writers.at(topic);
	writer << message;
	return true;
}
