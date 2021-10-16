#pragma once

#include "IRequestProtocolConverter.h"
#include "IRequestFactory.h"

namespace whisper::common::request {

class RequestProtocolConverter final: public IRequestProtocolConverter {
    Q_DISABLE_COPY(RequestProtocolConverter)
public:
    RequestProtocolConverter(const IRequestFactoryPtr& requestFactory);

    RequestBasePtr deserializeRequest(RequestType::Type requestType, const QByteArray& data) const override;

private:
    IRequestFactoryPtr requestFactory_;
};

} // namespace whisper::common::request
