#include "servercontroller.h"
#include "serversqlitedatastorage.h"
#include "dispatcher.h"

#include <connection.h>
#include <crypto.h>

using namespace whisper::common;

namespace whisper {
namespace server {

ServerController::ServerController(Connection* connection, ServerSqliteDataStorage* db, Dispatcher* dispatcher, QObject *parent)
    : Controller(parent)
    , db_(db)
    , dispatcher_(dispatcher)
{
    setConnection(connection);

    connect(&killTimer_, &QTimer::timeout, connection_, &Connection::close);
//    startKillTimer(1min);
}

void ServerController::startKillTimer(std::chrono::milliseconds timer) {
    killTimer_.start(timer);
}

void ServerController::stopKillTimer() {
    killTimer_.stop();
}

void ServerController::onPlainCommandReceived(SerializedCommand cmd) {
    switch (cmd.id_) {
        HANDLE_COMMAND(CS_HANDSHAKE_REQUEST);
        HANDLE_COMMAND(CS_HANDSHAKE_SOLUTION);
        HANDLE_COMMAND(CS_NEW_USER);
        HANDLE_COMMAND(CS_OLD_USER);
        HANDLE_COMMAND(CS_INTERACTIVE_CHALLENGE_REPLY);
        HANDLE_COMMAND(CC_ADD_CONTACT_REQUEST);
        HANDLE_COMMAND(CC_ADD_CONTACT_REQUEST_COMPLETED);
        HANDLE_COMMAND(CC_ADD_CONTACT_ACCEPT);
        HANDLE_COMMAND(CC_MESSAGE);
        default: {
            wError << "unexpected command";
        }
    }
}

void ServerController::onEncryptedCommandReceived(EncryptedCommand cmd){
    // TODO: decrypt and call onPlainCommandReceived
}

void ServerController::onConnectionStateChanged(QAbstractSocket::SocketState state) {
    if (state == QAbstractSocket::UnconnectedState) {
        setAuthorized(false);
        emit controllerClosed();
    }
}

DEFINE_SERVER_HANDLER(CS_HANDSHAKE_REQUEST, cmd) {
    if (!handshakeSuccessfull()) {
        const auto c = cmd.deserialize<CS_HANDSHAKE_REQUEST>();
        setDeviceCertificate(c.deviceCertificate);
        setDeviceCertificateHash(qHash(c.deviceCertificate));
        wDebug << deviceCertificateHash() << deviceCertificate().left(10);
        if (db_->deviceExists(c.deviceCertificate)) {
            connection_->send(SC_HANDSHAKE_SUCCESSFULL{});
            setHandshakeSuccessfull(true);
            wDebug << "successfull";
        } else {
            if (handshakeRetryCount() == 0) {
                const auto cr = Crypto::generateChallengeResponse();
                setHandshakeChallenge(cr.first);
                setExpectedHandshakeSolution(cr.second);
                wInfo << cr.second;
            }
            connection_->send(SC_HANDSHAKE_REPLY{ handshakeChallenge() });
        }
    }
}

DEFINE_SERVER_HANDLER(CS_HANDSHAKE_SOLUTION, cmd) {
    if (!handshakeSuccessfull()) {
        const auto c = cmd.deserialize<CS_HANDSHAKE_SOLUTION>();
        if (c.handshakeSolution == expectedHandshakeSolution()) {
            setHandshakeSuccessfull(true);
            setExpectedHandshakeSolution({});
            setHandshakeChallenge({});
            setHandshakeRetryCount(0);
            db_->rememberDevice(deviceCertificate());
            connection_->send(SC_HANDSHAKE_SUCCESSFULL{});
            stopKillTimer();
            wDebug << "successfull";
        } else {
            emit handshakeRetryCountChanged(++handshakeRetryCount_);
            if (handshakeRetryCount() > 3) {
                connection_->close();
                wDebug << "failed";
            } else {
                connection_->send(SC_HANDSHAKE_RETRY{});
                wDebug << "retry" << handshakeRetryCount();
            }
        }
    }
}

DEFINE_SERVER_HANDLER(CS_NEW_USER, cmd) {
    if (handshakeSuccessfull() && !authorized()) {
        const auto c = cmd.deserialize<CS_NEW_USER>();
        if (db_->userExists(c.userCertificate)) {
            connection_->close();
            wDebug << "user certificate is already known";
        } else {
            quint64 userId = 0;
            do {
                userId = Crypto::generateNewUserId();
            } while (db_->userIdExists(userId));
            setUserCertificate(c.userCertificate);
            setUserCertificateHash(qHash(c.userCertificate));
            setUserId(userId);
            const auto salt = Crypto::generateSalt();
            const auto passwordHash = Crypto::hashPassword(c.password, salt);
            db_->rememberNewUser(userId, c.userCertificate, deviceCertificate(), passwordHash, salt);
            setAuthorized(true);
            connection_->send(SC_NEW_USER_CREATED{ userId });
            connection_->send(SC_AUTHORIZED{});
        }
    }
}

DEFINE_SERVER_HANDLER(CS_OLD_USER, cmd) {
    if (handshakeSuccessfull() && !authorized()) {
        const auto c = cmd.deserialize<CS_OLD_USER>();
        if (db_->userIdExists(c.userId)) {
            // TODO: wipe password
            // TODO: safe password
            const auto storedPasswordHash = db_->getPasswordHash(c.userId);
            const auto salt = db_->getPasswordSalt(c.userId);
            const auto passwordHash = Crypto::hashPassword(c.password, salt);
            if (storedPasswordHash == passwordHash) {
                const bool deviceIsKnown = db_->deviceExists(deviceCertificate());
                if (deviceIsKnown) {
                    setUserCertificate(db_->getUserCertificate(c.userId));
                    setUserCertificateHash(qHash(userCertificate()));
                    setUserId(c.userId);
                    setAuthorized(true);
                } else {
                    // TODO: new device routine
                }
                connection_->send(SC_AUTHORIZED{});
            } else {
                // TODO: several attempts support
                connection_->close();
                wDebug << "fail wrong pass";
            }
        } else {
            connection_->close();
            wDebug << "fail we don't know u";
        }
    }
}

DEFINE_SERVER_HANDLER(CS_INTERACTIVE_CHALLENGE_REPLY, cmd) {
    if (handshakeSuccessfull() && !authorized()) {
        const auto c = cmd.deserialize<CS_INTERACTIVE_CHALLENGE_REPLY>();
        // TODO: do that aswell
    }
}

DEFINE_SERVER_HANDLER(CC_ADD_CONTACT_REQUEST, cmd) {
    if (handshakeSuccessfull() && authorized()) {
        const auto c = cmd.deserialize<CC_ADD_CONTACT_REQUEST>();
        dispatcher_->enqueueContactRequest(c.userId);
    }
}

DEFINE_SERVER_HANDLER(CC_ADD_CONTACT_REQUEST_COMPLETED, cmd) {
    if (handshakeSuccessfull() && authorized()) {
        const auto c = cmd.deserialize<CC_ADD_CONTACT_REQUEST_COMPLETED>();
        // TODO: do that aswell
    }
}

DEFINE_SERVER_HANDLER(CC_ADD_CONTACT_ACCEPT, cmd) {
    if (handshakeSuccessfull() && authorized()) {
        const auto c = cmd.deserialize<CC_ADD_CONTACT_ACCEPT>();
        dispatcher_->enqueueContactRequestAccepted(c.userId);
    }
}

DEFINE_SERVER_HANDLER(CC_MESSAGE, cmd) {
    if (handshakeSuccessfull() && authorized()) {
        const auto c = cmd.deserialize<CC_MESSAGE>();
        dispatcher_->enqueueMessage(c.userId, c.encryptedMessage);
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

} // namespace server
} // namespace whisper
