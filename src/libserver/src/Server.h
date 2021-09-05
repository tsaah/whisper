#pragma once

#include "libserver_export.h"

#include <QTcpServer>

namespace whisper::server {

class IServerConnectionFactory;

class WHISPER_LIBSERVER Server: public QTcpServer {
    Q_OBJECT
public:
    Server(IServerConnectionFactory* socketFactory, QObject* parent = nullptr);

protected:
    void incomingConnection(qintptr handle) override;

private:
    IServerConnectionFactory* socketFactory_{ nullptr };
};

} // namespace whisper::server
