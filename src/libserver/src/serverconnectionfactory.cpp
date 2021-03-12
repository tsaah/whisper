#include "serverconnectionfactory.h"

#include <connection.h>

namespace whisper {
namespace server {

QTcpSocket *ServerConnectionFactory::create(qintptr handle) {
    auto* socket = new common::Connection;
    if (socket != nullptr && socket->setSocketDescriptor(handle)) {
        return socket;
    }
    return nullptr;
}

} // namespace server
} // namespace whisper
