#pragma once

#include "IRequestProcessor.h"

#include <QHash>

namespace whisper::common::request {

class RequestProcessor: public IRequestProcessor {
    Q_DISABLE_COPY(RequestProcessor)
public:
    RequestProcessor();

    void processRequest(const RequestBasePtr& request) override;
    void addSubprocessor(RequestType::Type requestType, const RequestSubprocessorPtr& subprocessor) override;
    void removeSubprocessor(RequestType::Type requestType) override;
    void removeAllSubprocessors();
private:
    RequestSubprocessorPtr getSubprocessorForRequest(RequestType::Type requestType) const;
    QHash<RequestType::Type, RequestSubprocessorPtr> subprocessorHash_;
};

} // namespace whisper::common::request
