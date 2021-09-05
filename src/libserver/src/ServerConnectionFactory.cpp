#include "ServerConnectionFactory.h"

#include <Connection.h>

namespace whisper::server {

QTcpSocket *ServerConnectionFactory::create(qintptr handle) {
    auto* socket = new common::Connection;
    if (socket != nullptr && socket->setSocketDescriptor(handle)) {
        return socket;
    }
    return nullptr;
}

} // namespace whisper::server
