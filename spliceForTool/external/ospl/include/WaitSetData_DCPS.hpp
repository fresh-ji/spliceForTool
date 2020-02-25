#ifndef ISOCPP2_WAITSETDATA_H
#define ISOCPP2_WAITSETDATA_H

#include "dds/dds.hpp"

#include "WaitSetData.h"
#include "WaitSetDataSplDcps.h"

#include "org/opensplice/topic/TopicTraits.hpp"
#include "org/opensplice/topic/DataRepresentation.hpp"

namespace org { namespace opensplice { namespace topic {
template <>
class TopicTraits<WaitSetData::Msg>
{
public:
    static ::org::opensplice::topic::DataRepresentationId_t getDataRepresentationId()
    {
        return ::org::opensplice::topic::OSPL_REPRESENTATION;
    }

    static ::std::vector<os_uchar> getMetaData()
    {
        return ::std::vector<os_uchar>();
    }

    static ::std::vector<os_uchar> getTypeHash()
    {
        return ::std::vector<os_uchar>();
    }

    static ::std::vector<os_uchar> getExtentions()
    {
        return ::std::vector<os_uchar>();
    }

    static const char *getKeyList()
    {
        return "subjectId";
    }

    static const char *getTypeName()
    {
        return "WaitSetData::Msg";
    }

    static std::string getDescriptor()
    {
        const char *elements[] = {
            "<MetaData version=\"1.0.0\"><Module name=\"WaitSetData\"><Struct name=\"Msg\"><Member name=\"subjectId\">",
"<Long/></Member><Member name=\"systemId\"><String/></Member><Member name=\"time\"><Double/></Member><Member name=\"from\">",
"<String/></Member><Member name=\"topicName\"><String/></Member><Member name=\"content\"><String/></Member>",
"</Struct></Module></MetaData>"
        };
        std::string descriptor;
        descriptor.reserve(362);
        for (int i = 0; i < 4; i++) {
            descriptor.append(elements[i]);
        }

        return descriptor;
    }

    static copyInFunction getCopyIn()
    {
        return (copyInFunction) __WaitSetData_Msg__copyIn;
    }

    static copyOutFunction getCopyOut()
    {
        return (copyOutFunction) __WaitSetData_Msg__copyOut;
    }
};
}}}

namespace dds { namespace topic {
template <>
struct topic_type_name<WaitSetData::Msg>
{
    static std::string value()
    {
        return org::opensplice::topic::TopicTraits<WaitSetData::Msg>::getTypeName();
    }
};
}}

REGISTER_TOPIC_TYPE(WaitSetData::Msg)

#endif /* ISOCPP2_WAITSETDATA_H */
