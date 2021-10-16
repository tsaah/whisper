#pragma once

#include "IRequestSubprocessor.h"
#include "RequestBase.h"

#include <QSharedPointer>

namespace whisper::common::request {

class IRequestProcessor {
    Q_DISABLE_COPY(IRequestProcessor)
public:
    IRequestProcessor() = default;
    virtual ~IRequestProcessor() = default;

    virtual void processRequest(const RequestBasePtr& request) = 0;
    virtual void addSubprocessor(RequestType::Type requestType, const RequestSubprocessorPtr& subprocessor) = 0;
    virtual void removeSubprocessor(RequestType::Type requestType) = 0;
    virtual void removeAllSubprocessors() = 0;
};

using RequestProcessorPtr = QSharedPointer<IRequestProcessor>;

} // namespace whisper::common::request
