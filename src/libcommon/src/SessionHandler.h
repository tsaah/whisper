#pragma once

#include "Request/IRequestProcessorFactory.h"
#include "Request/IRequestProtocolConverter.h"

#include <QObject>

namespace whisper::common {

class SessionHandler final: public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(SessionHandler)
public:
    SessionHandler(const request::RequestProcessorFactoryPtr& requestProcessorFactory, const request::RequestProtocolConverterPtr& requestProtocolConverter);

public slots:
    void processData(const QByteArray& data);
};

} // namespace whisper::common
