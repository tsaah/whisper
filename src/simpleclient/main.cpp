#include <log.h>

#include <QGuiApplication>
//#include <QFontDatabase>
#include <QQmlApplicationEngine>
#include <QQmlContext>
//#include <QQuickStyle>
#include <QSslSocket>
#include <Connection.h>

#include <Crypto/Crypto.h>
#include <Crypto/PrivateKey.h>
#include <Crypto/PublicKey.h>

#include <Request/RequestFactory.h>
#include <Request/CSRequestNewUserConnect.h>
#include <Request/RequestProtocolConverter.h>
//using namespace whisper::client;

//namespace {
//    const auto glyphFont = ":/fonts/fontello.ttf";
//}

int main(int argc, char** argv) {
    if (!QSslSocket::supportsSsl()) {
        wError << "no openssl support";
        return -1;
    }

    whisper::common::crypto::PrivateKey clientKey;
    clientKey.generate();
    whisper::common::crypto::PublicKey clientPublicKey(clientKey);
    whisper::common::crypto::PrivateKey deviceKey;
    deviceKey.generate();
    whisper::common::crypto::PublicKey devicePublicKey(deviceKey);


    QGuiApplication::setApplicationName("Whisper");
    QGuiApplication::setOrganizationName("Whisper.org");

    QGuiApplication application(argc, argv);

//    if (QFontDatabase::addApplicationFont(glyphFont) == -1) {
//        wError << "no glyph font" << glyphFont << "found";
//        return -1;
//    }

    QQmlApplicationEngine engine;

//    ClientController controller("simpleclient_data_storage.sqlite");

//    qmlRegisterUncreatableType<ClientController>("Whisper.Controller", 1, 0, "Controller", QString("Controller is uncratable"));
//    engine.rootContext()->setContextProperty("controller", &controller);


    whisper::common::Connection c;
    c.connectToHost("127.0.0.1", 12345);
    {
        auto request = whisper::common::request::CSRequestNewUserConnectPtr::create();
        request->clientKey = clientPublicKey;
        request->deviceKey = devicePublicKey;
        c.send(request);
    }



//    c->send(CS_HANDSHAKE_REQUEST{ "device cert 2", 11, 12, 13 });






//    Whisper::Network::ClientConnection connection(new Whisper::Network::SocketFactory);
//    connection.setSecuritySettings(s);
//    qmlRegisterUncreatableType<Whisper::Network::ClientConnection>("ClientConnection", 1, 0, "CommandInterface", QString("ClientConnection is uncratable"));
//    engine.rootContext()->setContextProperty("clientConnection", &connection);


    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    return application.exec();
}
