#ifndef _WAITSETDATA_H_
#define _WAITSETDATA_H_

#include <dds/core/ddscore.hpp>

namespace WaitSetData
{
    class Msg OSPL_DDS_FINAL
    {
    public:
        int32_t subjectId_;
        std::string systemId_;
        double time_;
        std::string from_;
        std::string topicName_;
        std::string content_;

    public:
        Msg() :
                subjectId_(0),
                time_(0.0) {}

        explicit Msg(
            int32_t subjectId,
            const std::string& systemId,
            double time,
            const std::string& from,
            const std::string& topicName,
            const std::string& content) : 
                subjectId_(subjectId),
                systemId_(systemId),
                time_(time),
                from_(from),
                topicName_(topicName),
                content_(content) {}

        Msg(const Msg &_other) : 
                subjectId_(_other.subjectId_),
                systemId_(_other.systemId_),
                time_(_other.time_),
                from_(_other.from_),
                topicName_(_other.topicName_),
                content_(_other.content_) {}

#ifdef OSPL_DDS_CXX11
        Msg(Msg &&_other) : 
                subjectId_(::std::move(_other.subjectId_)),
                systemId_(::std::move(_other.systemId_)),
                time_(::std::move(_other.time_)),
                from_(::std::move(_other.from_)),
                topicName_(::std::move(_other.topicName_)),
                content_(::std::move(_other.content_)) {}

        Msg& operator=(Msg &&_other)
        {
            if (this != &_other) {
                subjectId_ = ::std::move(_other.subjectId_);
                systemId_ = ::std::move(_other.systemId_);
                time_ = ::std::move(_other.time_);
                from_ = ::std::move(_other.from_);
                topicName_ = ::std::move(_other.topicName_);
                content_ = ::std::move(_other.content_);
            }
            return *this;
        }
#endif

        Msg& operator=(const Msg &_other)
        {
            if (this != &_other) {
                subjectId_ = _other.subjectId_;
                systemId_ = _other.systemId_;
                time_ = _other.time_;
                from_ = _other.from_;
                topicName_ = _other.topicName_;
                content_ = _other.content_;
            }
            return *this;
        }

        bool operator==(const Msg& _other) const
        {
            return subjectId_ == _other.subjectId_ &&
                systemId_ == _other.systemId_ &&
                time_ == _other.time_ &&
                from_ == _other.from_ &&
                topicName_ == _other.topicName_ &&
                content_ == _other.content_;
        }

        bool operator!=(const Msg& _other) const
        {
            return !(*this == _other);
        }

        int32_t subjectId() const { return this->subjectId_; }
        int32_t& subjectId() { return this->subjectId_; }
        void subjectId(int32_t _val_) { this->subjectId_ = _val_; }
        const std::string& systemId() const { return this->systemId_; }
        std::string& systemId() { return this->systemId_; }
        void systemId(const std::string& _val_) { this->systemId_ = _val_; }
#ifdef OSPL_DDS_CXX11
        void systemId(std::string&& _val_) { this->systemId_ = _val_; }
#endif
        double time() const { return this->time_; }
        double& time() { return this->time_; }
        void time(double _val_) { this->time_ = _val_; }
        const std::string& from() const { return this->from_; }
        std::string& from() { return this->from_; }
        void from(const std::string& _val_) { this->from_ = _val_; }
#ifdef OSPL_DDS_CXX11
        void from(std::string&& _val_) { this->from_ = _val_; }
#endif
        const std::string& topicName() const { return this->topicName_; }
        std::string& topicName() { return this->topicName_; }
        void topicName(const std::string& _val_) { this->topicName_ = _val_; }
#ifdef OSPL_DDS_CXX11
        void topicName(std::string&& _val_) { this->topicName_ = _val_; }
#endif
        const std::string& content() const { return this->content_; }
        std::string& content() { return this->content_; }
        void content(const std::string& _val_) { this->content_ = _val_; }
#ifdef OSPL_DDS_CXX11
        void content(std::string&& _val_) { this->content_ = _val_; }
#endif
    };

}

#endif /* _WAITSETDATA_H_ */
