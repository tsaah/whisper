#include <connection.h>
#include <clientcommandprocessor.h>
#include <clientconnectionstate.h>
#include <clientsqlitedatastorage.h>

#include <QGuiApplication>
//#include <QFontDatabase>
#include <QQmlApplicationEngine>
#include <QQmlContext>
//#include <QQuickStyle>

using namespace whisper::common;
using namespace whisper::client;

//namespace {
//    const auto glyphFont = ":/fonts/fontello.ttf";
//}

int main(int argc, char** argv) {

    if (!QSslSocket::supportsSsl()) {
        wError << "no openssl support";
        return -1;
    }


    QGuiApplication::setApplicationName("Whisper");
    QGuiApplication::setOrganizationName("Whisper.org");

    QGuiApplication application(argc, argv);

//    if (QFontDatabase::addApplicationFont(glyphFont) == -1) {
//        wError << "no glyph font" << glyphFont << "found";
//        return -1;
//    }

    QQmlApplicationEngine engine;


    auto* c = new Connection;
    auto* s = new ClientConnectionState(c);
    auto* p = new ClientCommandProcessor(c);
    auto* d = new ClientSqliteDataStorage(c);
    QObject::connect(c, &Connection::commandReceived, [c, s, p, d](SerializedCommand command){
        p->processCommand(c, command, s, d);
    });

    qmlRegisterUncreatableType<Connection>("Whisper.Connection", 1, 0, "Connection", QString("Connection is uncratable"));
    engine.rootContext()->setContextProperty("connection", c);


//    c->connectToHost("127.0.0.1", 12345);

//    c->send(CS_HANDSHAKE_REQUEST{ "device cert 2", 11, 12, 13 });






//    Whisper::Network::ClientConnection connection(new Whisper::Network::SocketFactory);
//    connection.setSecuritySettings(s);
//    qmlRegisterUncreatableType<Whisper::Network::ClientConnection>("ClientConnection", 1, 0, "CommandInterface", QString("ClientConnection is uncratable"));
//    engine.rootContext()->setContextProperty("clientConnection", &connection);


    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    return application.exec();
}
