#pragma once

#include "RequestBase.h"

#include <QSharedPointer>

namespace whisper::common::request {

class IRequestProtocolConverter {
    Q_DISABLE_COPY(IRequestProtocolConverter)
public:
    IRequestProtocolConverter() = default;
    virtual ~IRequestProtocolConverter() = default;

    virtual RequestBasePtr deserializeRequest(request::RequestType::Type requestType, const QByteArray& data) const = 0;
};

using RequestProtocolConverterPtr = QSharedPointer<IRequestProtocolConverter>;

} // namespace whisper::common::request
