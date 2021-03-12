#include "server.h"
#include "iserverconnectionfactory.h"
#include "log.h"

#include <QTcpSocket>

namespace whisper {
namespace server {

Server::Server(IServerConnectionFactory *socketFactory, QObject *parent)
    : QTcpServer(parent)
    , socketFactory_(socketFactory)
{}

void Server::incomingConnection(qintptr handle) {
    auto* socket = socketFactory_->create(handle);
    if (socket != nullptr && socket->socketDescriptor() == handle) {
        wDebug << "new incoming connection" << handle << "added";
        addPendingConnection(socket);
    }
}

} // namespace server
} // namespace whisper
