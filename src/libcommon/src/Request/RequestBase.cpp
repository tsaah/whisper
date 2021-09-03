#include "RequestBase.h"

#include <QDataStream>

namespace whisper::common::request {

RequestBase::RequestBase(RequestType::Type type)
    : requestType_(type)
{
}

bool RequestBase::deserialize(const QByteArray& byteArray) {
    QDataStream stream(byteArray);
    stream.setVersion(QDataStream::Qt_5_15); // TODO: grab it from settings or other global source

    auto type = RequestType::Unknown;
    stream >> type;
    if (type != requestType()) {
        return false;
    }

    return deserializeSpecific(stream);
}

RequestType::Type RequestBase::prefetchRequestType(const QByteArray &byteArray)
{
    QDataStream stream(byteArray);
    stream.setVersion(QDataStream::Qt_5_15); // TODO: grab it from settings or other global source

    auto type = RequestType::Unknown;
    stream >> type;

    return type;
}

QByteArray RequestBase::serialize() const
{
    if (requestType_ == RequestType::Unknown) {
        return {};
    }

    QByteArray result;
    QDataStream stream(&result, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_15); // TODO: grab it from settings or other global source

    stream << requestType_;

    if (!serializeSpecific(stream)) {
        return {};
    }

    return result;
}

RequestType::Type RequestBase::requestType() const {
    return requestType_;
}

bool RequestBase::deserializeSpecific(QDataStream& stream)
{
    return false;
}

bool RequestBase::serializeSpecific(QDataStream& stream) const
{
    return {};
}

} // namespace whisper::common::request
