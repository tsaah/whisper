#pragma once

#include <QObject>
#include <QSharedPointer>

namespace whisper::common::request {

class RequestBase {
    Q_GADGET
    Q_DISABLE_COPY(RequestBase);
public:
    enum RequestType {
        Unknown,
        Test, // TODO: generate random values for types
    };
    Q_ENUM(RequestType);

    explicit RequestBase(RequestType type);
    virtual ~RequestBase() = default;

    virtual bool deserialize(const QByteArray& byteArray);
    virtual QByteArray serialize() const;

    RequestType requestType() const;
    QString lastError() const;

private:
    const RequestType requestType_;
    QString lastError_;
};

using RequestBasePtr = QSharedPointer<RequestBase>;

} // namespace whisper::common::request

Q_DECLARE_METATYPE(whisper::common::request::RequestBasePtr);
