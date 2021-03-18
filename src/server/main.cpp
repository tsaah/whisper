//#include "mock_connection.h"
#include <connection.h>
#include <log.h>
#include <serverconnectionfactory.h>
#include <server.h>
#include <packet.h>
#include <commandprocessor.h>
#include <servercommandprocessor.h>
#include <clientcommandprocessor.h>

#include <QCoreApplication>
#include <QTcpServer>

#include <functional>

using whisper::common::Connection;
using whisper::common::CommandProcessor;
using whisper::server::ServerConnectionFactory;
using whisper::server::Server;
using whisper::server::ServerCommandProcessor;
using whisper::client::ClientCommandProcessor;
using namespace whisper::common;

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
        QObject::connect(connection, &Connection::commandReceived, [connection](SerializedCommand command){
            auto* p = new ServerCommandProcessor(connection);
            p->processCommand(connection, command, nullptr);
        });
    });

    auto* c = new Connection;
    QObject::connect(c, &Connection::commandReceived, [c](SerializedCommand command){
        auto* p = new ClientCommandProcessor(c);
        p->processCommand(c, command, nullptr);
    });
    c->connectToHost("127.0.0.1", 12345);

    c->send(CS_HANDSHAKE_REQUEST{ "device cert", 11, 12, 13 });

    return application.exec();
}


#include "main.moc"
