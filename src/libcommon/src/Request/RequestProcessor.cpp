#include "log.h"

#include "RequestProcessor.h"

namespace whisper::common::request {

RequestProcessor::RequestProcessor()
    : IRequestProcessor()
{
}

void RequestProcessor::processRequest(const RequestBasePtr& request) {
    auto subprocessor = getSubprocessorForRequest(request->requestType());
    if (subprocessor) {
        subprocessor->processRequest(request);
    }
}

void RequestProcessor::addSubprocessor(RequestType::Type requestType, const RequestSubprocessorPtr& subprocessor) {
    subprocessorHash_.insert(requestType, subprocessor);
}

void RequestProcessor::removeSubprocessor(RequestType::Type requestType) {
    subprocessorHash_.remove(requestType);
}

void RequestProcessor::removeAllSubprocessors() {
    subprocessorHash_.clear();
}

RequestSubprocessorPtr RequestProcessor::getSubprocessorForRequest(RequestType::Type requestType) const {
    auto subprocessor = subprocessorHash_.value(requestType);
    if (!subprocessor) {
        wWarn << "We cannot handle that request type at the moment" << requestType;
    }
    return subprocessor;
}

} // namespace whisper::common::request
