#pragma once

#include "IRequestProcessor.h"

#include <QHash>

namespace whisper::common::request {

class RequestProcessor: public IRequestProcessor {
    Q_DISABLE_COPY(RequestProcessor)
public:
    RequestProcessor();

    void processRequest(const RequestBasePtr& request) override;
    void addSubProcessor(RequestType::Type requestType, const RequestSubProcessorPtr& subProcessor) override;
    void removeSubProcessor(RequestType::Type requestType) override;
    void removeAllSubProcessors();
private:
    RequestSubProcessorPtr getSubProcessorForRequest(RequestType::Type requestType) const;
    QHash<RequestType::Type, RequestSubProcessorPtr> subprocessorHash_;
};

} // namespace whisper::common::request
