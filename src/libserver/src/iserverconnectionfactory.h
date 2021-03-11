#ifndef ISERVERCONNECTIONFACTORY_H
#define ISERVERCONNECTIONFACTORY_H

#include "libserver_export.h"

class QTcpSocket;

namespace whisper {
namespace network {

class WHISPER_LIBSERVER IServerConnectionFactory {
public:
    ~IServerConnectionFactory() = default;
    virtual QTcpSocket* create(qintptr handle) = 0;
};

} // namespace network
} // namespace whisper

#endif // ISERVERCONNECTIONFACTORY_H
