#include "ServerController.h"
#include "ServerSqliteDataStorage.h"
#include "Dispatcher.h"

#include <Connection.h>
#include <Crypto/Crypto.h>

using namespace whisper::common;

namespace whisper::server {

ServerController::ServerController(Connection* connection, ServerSqliteDataStorage* db, Dispatcher* dispatcher, QObject *parent)
    : QObject(parent)
    , db_(db)
    , dispatcher_(dispatcher)
{
//    setConnection(connection);

//    connect(&killTimer_, &QTimer::timeout, connection_, &Connection::close);
//    startKillTimer(1min);
}

void ServerController::startKillTimer(std::chrono::milliseconds timer) {
    killTimer_.start(timer);
}

void ServerController::stopKillTimer() {
    killTimer_.stop();
}

void ServerController::onRequestReceived(const QByteArray &request)
{

}

void ServerController::onConnectionStateChanged(QAbstractSocket::SocketState state) {
    if (state == QAbstractSocket::UnconnectedState) {
//        setAuthorized(false);
        emit controllerClosed();
    }
}

bool ServerController::handshakeSuccessfull() const {
    return handshakeSuccessfull_;
}

int ServerController::handshakeRetryCount() const {
    return handshakeRetryCount_;
}

QString ServerController::expectedHandshakeSolution() const {
    return expectedHandshakeSolution_;
}

QString ServerController::handshakeChallenge() const {
    return handshakeChallenge_;
}

void ServerController::setHandshakeSuccessfull(bool handshakeSuccessfull) {
    if (handshakeSuccessfull_ == handshakeSuccessfull) { return; }
    handshakeSuccessfull_ = handshakeSuccessfull;
    emit handshakeSuccessfullChanged(handshakeSuccessfull_);
}

void ServerController::setHandshakeRetryCount(int handshakeRetryCount) {
    if (handshakeRetryCount_ == handshakeRetryCount) { return; }
    handshakeRetryCount_ = handshakeRetryCount;
    emit handshakeRetryCountChanged(handshakeRetryCount_);
}

void ServerController::setExpectedHandshakeSolution(QString expectedHandshakeSolution) {
    if (expectedHandshakeSolution_ == expectedHandshakeSolution) { return; }
    expectedHandshakeSolution_ = expectedHandshakeSolution;
    emit expectedHandshakeSolutionChanged(expectedHandshakeSolution_);
}

void ServerController::setHandshakeChallenge(QString handshakeChallenge) {
    if (handshakeChallenge_ == handshakeChallenge) { return; }
    handshakeChallenge_ = handshakeChallenge;
    emit handshakeChallengeChanged(handshakeChallenge_);
}

} // namespace whisper::server
