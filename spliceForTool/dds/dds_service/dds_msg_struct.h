#ifndef DDSMSGSTRUCT_H_
#define DDSMSGSTRUCT_H_

typedef struct _MsgData{
	long subjectId;
	std::string systemId;
	double time;
	std::string from;
	std::string topicName;
	std::string content;
}MsgData;

#endif