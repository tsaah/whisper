#ifndef WHISPER_SERVER_SERVERCONTROLLER_H
#define WHISPER_SERVER_SERVERCONTROLLER_H

#include "libserver_export.h"

#include <controller.h>

#include <QObject>
#include <QTimer>

#include <chrono>

using namespace std::chrono_literals;

namespace whisper {
namespace server {

class ServerSqliteDataStorage;
class Dispatcher;

class WHISPER_LIBSERVER ServerController: public common::Controller {
    Q_OBJECT
    Q_PROPERTY(bool handshakeSuccessfull READ handshakeSuccessfull WRITE setHandshakeSuccessfull NOTIFY handshakeSuccessfullChanged)
    Q_PROPERTY(int handshakeRetryCount READ handshakeRetryCount WRITE setHandshakeRetryCount NOTIFY handshakeRetryCountChanged)
    Q_PROPERTY(QString expectedHandshakeSolution READ expectedHandshakeSolution WRITE setExpectedHandshakeSolution NOTIFY expectedHandshakeSolutionChanged)
    Q_PROPERTY(QString handshakeChallenge READ handshakeChallenge WRITE setHandshakeChallenge NOTIFY handshakeChallengeChanged)
public:
    explicit ServerController(common::Connection* connection, ServerSqliteDataStorage* db, Dispatcher* dispatcher, QObject* parent = nullptr);

    void startKillTimer(std::chrono::milliseconds timer);
    void stopKillTimer();

protected slots:
    void onPlainCommandReceived(common::SerializedCommand cmd) override;
    void onEncryptedCommandReceived(common::EncryptedCommand cmd) override;
    void onConnectionStateChanged(QAbstractSocket::SocketState state) override;

    DECLARE_HANDLER(CS_HANDSHAKE_REQUEST, cmd);
    DECLARE_HANDLER(CS_HANDSHAKE_SOLUTION, cmd);
    DECLARE_HANDLER(CS_NEW_USER, cmd);
    DECLARE_HANDLER(CS_OLD_USER, cmd);
    DECLARE_HANDLER(CS_INTERACTIVE_CHALLENGE_REPLY, cmd);
    DECLARE_HANDLER(CC_ADD_CONTACT_REQUEST, cmd);
    DECLARE_HANDLER(CC_ADD_CONTACT_REQUEST_COMPLETED, cmd);
    DECLARE_HANDLER(CC_ADD_CONTACT_ACCEPT, cmd);
    DECLARE_HANDLER(CC_MESSAGE, cmd);

signals:
    void controllerClosed();

private:
    ServerSqliteDataStorage* db_{ nullptr };
    Dispatcher* dispatcher_{ nullptr };
    QTimer killTimer_;



public:
    bool handshakeSuccessfull() const;
    int handshakeRetryCount() const;
    QString expectedHandshakeSolution() const;
    QString handshakeChallenge() const;

private slots:
    void setHandshakeSuccessfull(bool handshakeSuccessfull);
    void setHandshakeRetryCount(int handshakeRetryCount);
    void setExpectedHandshakeSolution(QString expectedHandshakeSolution);
    void setHandshakeChallenge(QString handshakeChallenge);

signals:
    void handshakeSuccessfullChanged(bool handshakeSuccessfull);
    void handshakeRetryCountChanged(int handshakeRetryCount);
    void expectedHandshakeSolutionChanged(QString expectedHandshakeSolution);
    void handshakeChallengeChanged(QString handshakeChallenge);

private:
    bool handshakeSuccessfull_{ false };
    int handshakeRetryCount_{ 0 };
    QString expectedHandshakeSolution_;
    QString handshakeChallenge_;
};

} // namespace server
} // namespace whisper

#define DEFINE_SERVER_HANDLER(c, v) void ServerController::handle_##c(const SerializedCommand &v)

#endif // WHISPER_SERVER_SERVERCONTROLLER_H
