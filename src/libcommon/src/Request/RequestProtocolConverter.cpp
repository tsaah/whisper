#include "RequestProtocolConverter.h"

namespace whisper::common::request {

RequestProtocolConverter::RequestProtocolConverter(const IRequestFactoryPtr &requestFactory)
    : IRequestProtocolConverter()
    , requestFactory_(requestFactory)
{

}

RequestBasePtr RequestProtocolConverter::deserializeRequest(request::RequestType::Type requestType,const QByteArray& data) const {
    if (requestType == RequestType::Unknown) {
        return {};
    }

    auto request = requestFactory_->create(requestType);
    if (request.isNull()) {
        return {};
    }

    const auto deserializeResult = request->deserialize(data);
    if (!deserializeResult) {
        return {};
    }
    return request;
}

} // namespace whisper::common::request
