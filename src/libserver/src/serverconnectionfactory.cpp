#include "serverconnectionfactory.h"

#include <connection.h>

namespace whisper {
namespace network {

QTcpSocket *ServerConnectionFactory::create(qintptr handle) {
    auto* socket = new Connection;
    if (socket != nullptr && socket->setSocketDescriptor(handle)) {
        return socket;
    }
    return nullptr;
}

} // namespace network
} // namespace whisper
