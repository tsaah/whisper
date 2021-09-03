#pragma once

#include "IRequestProtocolConverter.h"
#include "IRequestFactory.h"

namespace whisper::common::request {

class RequestProtocolConverter final: public IRequestProtocolConverter {
    Q_DISABLE_COPY(RequestProtocolConverter)
public:
    RequestProtocolConverter(const RequestFactoryPtr& requestFactory);

    RequestBasePtr deserializeRequest(const QByteArray& data) const override;
    QByteArray serializeRequest(const RequestBasePtr& request) const override;

private:
    RequestFactoryPtr requestFactory_;
};

} // namespace whisper::common::request
