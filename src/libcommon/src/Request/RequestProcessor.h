#pragma once

#include "IRequestProcessor.h"

#include <QHash>

namespace whisper::common::request {

class RequestProcessor: public IRequestProcessor {
    Q_DISABLE_COPY(RequestProcessor)
public:
    RequestProcessor();

    void processRequest(const RequestBasePtr& request) override;
    void addSubProcessor(RequestBase::RequestType requestType, const RequestSubProcessorPtr& subProcessor) override;
    void removeSubProcessor(RequestBase::RequestType requestType) override;
    void removeAllSubProcessors();
private:
    RequestSubProcessorPtr getSubProcessorForRequest(RequestBase::RequestType requestType) const;
    QHash<RequestBase::RequestType, RequestSubProcessorPtr> subprocessorHash_;
};

} // namespace whisper::common::request
