#include "log.h"

#include "RequestProcessor.h"

namespace whisper::common::request {

RequestProcessor::RequestProcessor()
    : IRequestProcessor()
{
}

void RequestProcessor::processRequest(const RequestBasePtr& request) {
    auto subProcessor = getSubProcessorForRequest(request->requestType());
    if (subProcessor) {
        subProcessor->processRequest(request);
    }
}

void RequestProcessor::addSubProcessor(RequestBase::RequestType requestType, const RequestSubProcessorPtr &subProcessor) {
    subprocessorHash_.insert(requestType, subProcessor);
}

void RequestProcessor::removeSubProcessor(RequestBase::RequestType requestType) {
    subprocessorHash_.remove(requestType);
}

void RequestProcessor::removeAllSubProcessors() {
    subprocessorHash_.clear();
}

RequestSubProcessorPtr RequestProcessor::getSubProcessorForRequest(RequestBase::RequestType requestType) const {
    auto subProcessor = subprocessorHash_.value(requestType);
    if (!subProcessor) {
        wWarn << "We cannot handle that request type at the moment" << requestType;
    }
    return subProcessor;
}

} // namespace whisper::common::request
