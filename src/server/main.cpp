#include <connection.h>
#include <controller.h>
#include <dispatcher.h>
#include <log.h>
#include <server.h>
#include <serverconnectionfactory.h>

#include <QCoreApplication>

using namespace whisper::common;
using namespace whisper::server;

int main(int argc, char** argv) {
    qsrand(QDateTime::currentMSecsSinceEpoch());

    QCoreApplication application(argc, argv);

    ServerConnectionFactory socketFactory;
    auto* server = new Server(&socketFactory, &application);
    const auto success = server->listen(QHostAddress::Any, 12345);

    Q_ASSERT_X(success, __FUNCTION__, "failed to start listen");
    if (!success) {
        wDebug << "failed to listen";
        return -1;
    }

    auto* dispatcher = new Dispatcher(&application);

    wDebug << "listening";
    QObject::connect(server, &Server::newConnection, [server, dispatcher]{
        wDebug << "new connection";
        auto* connection = reinterpret_cast<Connection*>(server->nextPendingConnection());
        dispatcher->addConnection(connection);
    });

    return application.exec();
}


