#pragma once

namespace whisper::common {

class SessionHandler final: public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(SessionHandler)
public:
    SessionHandler(const RequestProcessorFactoryPtr& requestProcessorFactory, const RequestProtocolConverter& requestProtocolConverter);

public slots:
    void processData(const QByteArray& data);


};

} // namespace whisper::common
