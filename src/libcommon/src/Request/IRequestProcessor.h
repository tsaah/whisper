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
    virtual void addSubProcessor(RequestBase::RequestType requestType, const RequestSubProcessorPtr& subProcessor) = 0;
    virtual void removeSubProcessor(RequestBase::RequestType requestType) = 0;
    virtual void removeAllSubProcessors() = 0;
};

using RequestProcessorPtr = QSharedPointer<IRequestProcessor>;

} // namespace whisper::common::request
