#include "RequestProtocolConverter.h"

namespace whisper::common::request {

RequestProtocolConverter::RequestProtocolConverter(const RequestFactoryPtr &requestFactory)
    : IRequestProtocolConverter()
    , requestFactory_(requestFactory)
{

}

RequestBasePtr RequestProtocolConverter::deserializeRequest(const QByteArray& data) const {
    const auto requestType = RequestBase::prefetchRequestType(data);
    if (requestType == RequestType::Unknown) {
        return {};
    }
    return requestFactory_->create(requestType);
}

QByteArray RequestProtocolConverter::serializeRequest(const RequestBasePtr& request) const {
    if (!request) {
        return {};
    }
    return request->serialize();
}

} // namespace whisper::common::request
