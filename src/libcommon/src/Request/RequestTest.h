#pragma once

#include "RequestBase.h"

namespace whisper::common::request {

class RequestTest: public RequestBase {
    Q_DISABLE_COPY(RequestTest);
public:
    RequestTest();

    bool deserialize(const QByteArray &byteArray);

    quint64 uint64Value() const;
    const QString &stringValue() const;
    const QByteArray &byteArrayValue() const;

private:
    quint64 uint64Value_{ 0 };
    QString stringValue_;
    QByteArray byteArrayValue_;
};

} // namespace whisper::common::request
