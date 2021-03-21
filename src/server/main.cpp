//#include "mock_connection.h"
#include <connection.h>
#include <log.h>
#include <serverconnectionfactory.h>
#include <server.h>
#include <packet.h>
#include <commandprocessor.h>
#include <servercommandprocessor.h>
#include <clientcommandprocessor.h>
#include <serverconnectionstate.h>
#include <clientconnectionstate.h>

#include <QCoreApplication>

using whisper::common::Connection;
using whisper::common::CommandProcessor;

using whisper::server::Server;
using whisper::server::ServerConnectionFactory;
using whisper::server::ServerCommandProcessor;
using whisper::server::ServerConnectionState;

using whisper::client::ClientCommandProcessor;
using whisper::client::ClientConnectionState;

using namespace whisper::common;

class IDataStorage {
public:
    virtual ~IDataStorage() = default;

    virtual bool isDeviceKnown(const QByteArray& deviceCert) const = 0;
};

int main(int argc, char** argv) {
    QCoreApplication application(argc, argv);

    ServerConnectionFactory socketFactory;
    auto* server = new Server(&socketFactory, &application);
    const auto success = server->listen(QHostAddress::Any, 12345);

    if (!success) {
        wDebug << "failed to listen";
        return -1;
    }

    wDebug << "listening";
    QObject::connect(server, &Server::newConnection, [server]{
        auto* connection = reinterpret_cast<Connection*>(server->nextPendingConnection());
        auto* s = new ServerConnectionState(connection);
        auto* p = new ServerCommandProcessor(connection);
        QObject::connect(connection, &Connection::commandReceived, [connection, s, p](SerializedCommand command){
            p->processCommand(connection, command, s);
        });
    });

    auto* c = new Connection;
    auto* s = new ClientConnectionState(c);
    auto* p = new ClientCommandProcessor(c);
    QObject::connect(c, &Connection::commandReceived, [c, s, p](SerializedCommand command){
        p->processCommand(c, command, s);
    });
    c->connectToHost("127.0.0.1", 12345);

    c->send(CS_HANDSHAKE_REQUEST{ "device cert", 11, 12, 13 });

    return application.exec();
}


#include "main.moc"
