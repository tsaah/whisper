#include "mock_connection.h"
#include "connection.h"
#include "log.h"
#include "serverconnectionfactory.h"
#include "server.h"

#include <QCoreApplication>
#include <QTcpServer>

#include <functional>



using namespace whisper::network;

class CommandProcessor: public QObject { // consider using one per connection or in connection
    Q_OBJECT
public:
    CommandProcessor(QObject* parent = nullptr)
        : QObject(parent)
    {
        init();
    }
    QHash<quint64, std::function<void(Connection* connection, const QByteArray&)>> handlers_;

    void init() { // consider mutating available handlers based on state
        handlers_[1] = [](Connection* connection, const QByteArray& command) {
            connection->send(2, "REPLY");
        };
        handlers_[2] = [](Connection* connection, const QByteArray& command) {
//            connection->send(2, "REPLY");
            wDebug << "ALL FINE";
        };
    }
    void process(Connection* connection, quint64 commandId, const QByteArray& command) {
        if (handlers_.contains(commandId)) {
            handlers_[commandId](connection, command);
        } else {
            wDebug << "wrong command";
            connection->close();
            connection->deleteLater();
        }
    }
};

#include "main.moc"

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
        QObject::connect(connection, &Connection::packetReceived, [connection](quint64 commandId, QByteArray command){
            CommandProcessor p;
            p.process(connection, commandId, command);
        });
    });

    Connection c;
    QObject::connect(&c, &Connection::packetReceived, [&c](quint64 commandId, QByteArray command){
        CommandProcessor p;
        p.process(&c, commandId, command);
    });
    c.connectToHost("127.0.0.1", 12345);
    c.send(1, "test");

    return application.exec();
}
