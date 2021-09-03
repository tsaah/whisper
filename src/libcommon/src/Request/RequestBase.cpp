#include "RequestBase.h"

namespace whisper::common::request {

RequestBase::RequestBase(RequestType type)
    : requestType_(type)
{
}

bool RequestBase::deserialize(const QByteArray& byteArray) {
    return false;
}

QByteArray RequestBase::serialize() const
{
    return {};
}

RequestBase::RequestType RequestBase::requestType() const {
    return requestType_;
}

QString RequestBase::lastError() const {
    return lastError_;
}

} // namespace whisper::common::request
