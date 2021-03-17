//#include "mock_connection.h"
#include <connection.h>
#include <log.h>
#include <serverconnectionfactory.h>
#include <server.h>
#include <packet.h>
#include <commandprocessor.h>

#include <QCoreApplication>
#include <QTcpServer>

#include <functional>

using whisper::common::Connection;
using whisper::common::CommandProcessor;
using whisper::server::ServerConnectionFactory;
using whisper::server::Server;
using namespace whisper::common;

//class CommandProcessor: public QObject { // consider using one per connection or in connection
//    Q_OBJECT
//public:
//    CommandProcessor(QObject* parent = nullptr)
//        : QObject(parent)
//    {
//        init();
//    }

//    QHash<quint64, std::function<void(Connection* connection, const SerializedCommand&)>> handlers_;

//    void init() { // consider mutating available handlers based on state
//        handlers_[command::CS_HANDSHAKE_REQUEST] = [](Connection* connection, const SerializedCommand& command) {
//            const auto cmd = command.deserialize<CS_HANDSHAKE_REQUEST>();
//            connection->send(CS_HANDSHAKE_SOLUTION{ "solution" });
//            wDebug << "ALL FINE1" << cmd.deviceCertificate_ << cmd.versionMajor_ << cmd.versionMinor_ << cmd.versionBuild_;
//        };
//        handlers_[command::CS_HANDSHAKE_SOLUTION] = [](Connection* connection, const SerializedCommand& command) {
//            const auto cmd = command.deserialize<CS_HANDSHAKE_SOLUTION>();
////            connection->send(2, "REPLY");
//            wDebug << "ALL FINE2" << cmd.handshakeSolution_;
//        };
//    }
//    void process(Connection* connection, const SerializedCommand& command) {
//        if (handlers_.contains(command.id_)) {
//            handlers_[command.id_](connection, command);
//        } else {
//            wDebug << "wrong command";
//            connection->close();
//            connection->deleteLater();
//        }
//    }
//};



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
            CommandProcessor p;
            p.insertHandler(command::CS_HANDSHAKE_REQUEST, [](Connection* connection, const SerializedCommand& serializedCommand, ConnectionState* state){
                const auto cmd = serializedCommand.deserialize<CS_HANDSHAKE_REQUEST>();
                connection->send(CS_HANDSHAKE_SOLUTION{ "solution" });
                wDebug << "ALL FINE1" << cmd.deviceCertificate_ << cmd.versionMajor_ << cmd.versionMinor_ << cmd.versionBuild_;
            });
            p.processCommand(connection, command, nullptr);
        });
    });

    auto* c = new Connection;
    QObject::connect(c, &Connection::commandReceived, [&c](SerializedCommand command){
        CommandProcessor p;
        p.insertHandler(command::CS_HANDSHAKE_SOLUTION, [](Connection* connection, const SerializedCommand& serializedCommand, ConnectionState* state){
            const auto cmd = serializedCommand.deserialize<CS_HANDSHAKE_SOLUTION>();
    //        connection->send(2, "REPLY");
            wDebug << "ALL FINE2" << cmd.handshakeSolution_;
        });
        p.processCommand(c, command, nullptr);
    });
    c->connectToHost("127.0.0.1", 12345);

    c->send(CS_HANDSHAKE_REQUEST{ "device cert", 11, 12, 13 });

    return application.exec();
}


#include "main.moc"
