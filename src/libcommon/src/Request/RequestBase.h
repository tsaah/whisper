#pragma once

#include "RequestType.h"

#include <QSharedPointer>

namespace whisper::common::request {

class RequestBase {
    Q_DISABLE_COPY(RequestBase);
public:
    explicit RequestBase(RequestType::Type type);
    virtual ~RequestBase() = default;

    QByteArray serialize() const;
    bool deserialize(const QByteArray& byteArray);
    static RequestType::Type prefetchRequestType(const QByteArray& byteArray);

    RequestType::Type requestType() const;

protected:
    virtual bool serializeSpecific(QDataStream& stream) const;
    virtual bool deserializeSpecific(QDataStream &stream);

private:
    const RequestType::Type requestType_;
};

using RequestBasePtr = QSharedPointer<RequestBase>;

} // namespace whisper::common::request

Q_DECLARE_METATYPE(whisper::common::request::RequestBasePtr);

#define BEGIN_REQUEST_DEFINITION(Prefix, Name) \
    namespace whisper::common::request { \
    class Prefix##Request##Name final: public RequestBase { \
        Q_DISABLE_COPY(Prefix##Request##Name); \
    public: \
        Prefix##Request##Name(): RequestBase(RequestType::Type::Prefix##Name) {} \


#define REQUEST_SERIALIZE_IMPLEMENTATION(Prefix, Name) \
    protected: \
    bool serializeSpecific(QDataStream &stream) const override \


#define REQUEST_DESERIALIZE_IMPLEMENTATION(Prefix, Name) \
    protected: \
    bool deserializeSpecific(QDataStream &stream) override \


#define END_REQUEST_DEFINITION(Prefix, Name) \
    }; \
    using Prefix##Request##Name##Ptr = QSharedPointer<Prefix##Request##Name>; \
    } \


#define BEGIN_CS_REQUEST_DEFINITION(Name) BEGIN_REQUEST_DEFINITION(CS, Name)
#define REQUEST_CS_SERIALIZE_IMPLEMENTATION(Name) REQUEST_SERIALIZE_IMPLEMENTATION(CS, Name)
#define REQUEST_CS_DESERIALIZE_IMPLEMENTATION(Name) REQUEST_DESERIALIZE_IMPLEMENTATION(CS, Name)
#define END_CS_REQUEST_DEFINITION(Name) END_REQUEST_DEFINITION(CS, Name)

#define BEGIN_SC_REQUEST_DEFINITION(Name) BEGIN_REQUEST_DEFINITION(SC, Name)
#define REQUEST_SC_SERIALIZE_IMPLEMENTATION(Name) REQUEST_SERIALIZE_IMPLEMENTATION(SC, Name)
#define REQUEST_SC_DESERIALIZE_IMPLEMENTATION(Name) REQUEST_DESERIALIZE_IMPLEMENTATION(SC, Name)
#define END_SC_REQUEST_DEFINITION(Name) END_REQUEST_DEFINITION(SC, Name)
