#ifndef ISERVERCONNECTIONFACTORY_H
#define ISERVERCONNECTIONFACTORY_H

#include "libserver_export.h"

class QTcpSocket;

namespace whisper {
namespace server {

class WHISPER_LIBSERVER IServerConnectionFactory {
public:
    ~IServerConnectionFactory() = default;
    virtual QTcpSocket* create(qintptr handle) = 0;
};

} // namespace server
} // namespace whisper

#endif // ISERVERCONNECTIONFACTORY_H
