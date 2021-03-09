#ifndef ISERVERCONNECTIONFACTORY_H
#define ISERVERCONNECTIONFACTORY_H

#include "server_export.h"

class QTcpSocket;

namespace whisper {
namespace network {

class WHISPER_SERVER_LIB IServerConnectionFactory {
public:
    ~IServerConnectionFactory() = default;
    virtual QTcpSocket* create(qintptr handle) = 0;
};

} // namespace network
} // namespace whisper

#endif // ISERVERCONNECTIONFACTORY_H
