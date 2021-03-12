#ifndef SERVER_H
#define SERVER_H

#include "libserver_export.h"

#include <QTcpServer>

namespace whisper {
namespace server {

class IServerConnectionFactory;

class WHISPER_LIBSERVER Server: public QTcpServer {
    Q_OBJECT
public:
    Server(IServerConnectionFactory* socketFactory, QObject* parent = nullptr);

protected:
    void incomingConnection(qintptr handle) override;

private:
    IServerConnectionFactory* socketFactory_{ nullptr };
};

} // namespace server
} // namespace whisper

#endif // SERVER_H
