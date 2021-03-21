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
#include <datastorage.h>
#include <serversqlitedatastorage.h>
#include <clientsqlitedatastorage.h>

#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QVariant>
#include <QSqlDatabase>
#include <QSqlQuery>

using whisper::common::Connection;
using whisper::common::CommandProcessor;
using whisper::common::DataStorage;

using whisper::server::Server;
using whisper::server::ServerConnectionFactory;
using whisper::server::ServerCommandProcessor;
using whisper::server::ServerConnectionState;
using whisper::server::ServerSqliteDataStorage;

using whisper::client::ClientCommandProcessor;
using whisper::client::ClientConnectionState;
using whisper::client::ClientSqliteDataStorage;

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
        wDebug << "new connection";
        auto* connection = reinterpret_cast<Connection*>(server->nextPendingConnection());
        auto* s = new ServerConnectionState(connection);
        auto* p = new ServerCommandProcessor(connection);
        auto* d = new ServerSqliteDataStorage(connection);
        QObject::connect(connection, &Connection::commandReceived, [connection, s, p, d](SerializedCommand command){
            p->processCommand(connection, command, s, d);
        });
    });

    auto* c = new Connection;
    auto* s = new ClientConnectionState(c);
    auto* p = new ClientCommandProcessor(c);
    auto* d = new ClientSqliteDataStorage(c);
    QObject::connect(c, &Connection::commandReceived, [c, s, p, d](SerializedCommand command){
        p->processCommand(c, command, s, d);
    });
    c->connectToHost("127.0.0.1", 12345);

    c->send(CS_HANDSHAKE_REQUEST{ "device cert", 11, 12, 13 });

    return application.exec();
}


#include "main.moc"
