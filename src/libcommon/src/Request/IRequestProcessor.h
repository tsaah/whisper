#pragma once

#include "IRequestSubProcessor.h"
#include "RequestBase.h"

#include <QSharedPointer>

namespace whisper::common::request {

class IRequestProcessor {
    Q_DISABLE_COPY(IRequestProcessor)
public:
    IRequestProcessor() = default;
    virtual ~IRequestProcessor() = default;

    virtual void processRequest(const RequestBasePtr& request) = 0;
    virtual void addSubProcessor(RequestType::Type requestType, const RequestSubProcessorPtr& subProcessor) = 0;
    virtual void removeSubProcessor(RequestType::Type requestType) = 0;
    virtual void removeAllSubProcessors() = 0;
};

using RequestProcessorPtr = QSharedPointer<IRequestProcessor>;

} // namespace whisper::common::request
