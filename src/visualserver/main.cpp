#include <Connection.h>
//#include <Controller.h>
#include <Dispatcher.h>
#include <log.h>
#include <Server.h>
#include <ServerConnectionFactory.h>
#include <ServerModel.h>
#include <Request/RequestProcessor.h>
#include <Request/RequestFactory.h>
#include <Request/RequestProtocolConverter.h>
#include <Request/SubprocessorCSRequestNewUserConnect.h>

#include <QApplication>
#include <QTreeView>

using namespace whisper::common;
using namespace whisper::server;



int main(int argc, char** argv) {
    QApplication application(argc, argv);

    ServerConnectionFactory socketFactory;
    auto* server = new Server(&socketFactory, &application);
    const auto success = server->listen(QHostAddress::Any, 12345);

    Q_ASSERT_X(success, __FUNCTION__, "failed to start listen");
    if (!success) {
        wDebug << "failed to listen";
        return -1;
    }

    auto* dispatcher = new Dispatcher(&application);
    auto* requestProcessor = new request::RequestProcessor;
    requestProcessor->addSubprocessor(request::RequestType::CSNewUserConnect, request::SubprocessorCSRequestNewUserConnectPtr::create());
    auto requestFactory = request::RequestFactoryPtr::create();
    auto* protocolConverter = new request::RequestProtocolConverter(requestFactory);

    wDebug << "listening";
    QObject::connect(server, &Server::newConnection, server, [server, dispatcher, requestProcessor, protocolConverter]{
        wDebug << "new connection";
        auto* connection = reinterpret_cast<Connection*>(server->nextPendingConnection());
        QObject::connect(connection, &Connection::requestRecieved, connection, [connection, requestProcessor, protocolConverter](request::RequestType::Type requestType, QByteArray payload){
            wDebug << "incoming request" << requestType;
            const auto request = protocolConverter->deserializeRequest(requestType, payload);
            requestProcessor->processRequest(request);
        });
        dispatcher->addConnection(connection);
    });

    auto* serverModel = new ServerModel(dispatcher);

    auto* treeView = new QTreeView;
    treeView->setAnimated(false);
    treeView->setAllColumnsShowFocus(true);
    treeView->setExpandsOnDoubleClick(true);
    treeView->setRootIsDecorated(true);

    treeView->setModel(serverModel);
    treeView->show();

    return application.exec();
}


