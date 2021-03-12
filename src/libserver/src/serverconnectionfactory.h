#ifndef SERVERCONNECTIONFACTORY_H
#define SERVERCONNECTIONFACTORY_H

#include "libserver_export.h"
#include "iserverconnectionfactory.h"

namespace whisper {
namespace server {

class WHISPER_LIBSERVER ServerConnectionFactory: public IServerConnectionFactory {
public:
    QTcpSocket* create(qintptr handle) override;
};

} // namespace server
} // namespace whisper

#endif // SERVERCONNECTIONFACTORY_H
