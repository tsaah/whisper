#ifndef SERVER_H
#define SERVER_H

#include "server_export.h"

#include <QTcpServer>

namespace whisper {
namespace network {

class IServerConnectionFactory;

class WHISPER_SERVER_LIB Server: public QTcpServer {
    Q_OBJECT
public:
    Server(IServerConnectionFactory* socketFactory, QObject* parent = nullptr);

protected:
    void incomingConnection(qintptr handle) override;

private:
    IServerConnectionFactory* socketFactory_{ nullptr };
};

} // namespace network
} // namespace whisper

#endif // SERVER_H
