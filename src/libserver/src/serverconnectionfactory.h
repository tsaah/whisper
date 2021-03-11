#ifndef SERVERCONNECTIONFACTORY_H
#define SERVERCONNECTIONFACTORY_H

#include "libserver_export.h"
#include "iserverconnectionfactory.h"

namespace whisper {
namespace network {

class WHISPER_LIBSERVER ServerConnectionFactory: public IServerConnectionFactory {
public:
    QTcpSocket* create(qintptr handle) override;
};

} // namespace network
} // namespace whisper

#endif // SERVERCONNECTIONFACTORY_H
