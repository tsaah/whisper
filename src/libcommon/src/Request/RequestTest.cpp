#include "RequestTest.h"

#include <QDataStream>

namespace whisper::common::request {

RequestTest::RequestTest()
    : RequestBase(RequestBase::RequestType::Test)
{
}

bool RequestTest::deserialize(const QByteArray &byteArray)
{
    QDataStream stream(byteArray);
    stream.setVersion(QDataStream::Qt_5_15); // TODO: grab it from settings or other global source

    auto type = RequestType::Unknown;
    stream >> type;
    if (type != requestType()) {
        return false;
    }

    stream >> uint64Value_ >> stringValue_ >> byteArrayValue_;

    return true;
}

quint64 RequestTest::uint64Value() const
{
    return uint64Value_;
}

const QString &RequestTest::stringValue() const
{
    return stringValue_;
}

const QByteArray &RequestTest::byteArrayValue() const
{
    return byteArrayValue_;
}

} // namespace whisper::common::request
