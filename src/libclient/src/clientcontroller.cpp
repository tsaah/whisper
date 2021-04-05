#include "clientcontroller.h"
#include "clientsqlitedatastorage.h"

#include <connection.h>
#include <crypto.h>

using namespace whisper::common;

namespace whisper {
namespace client {

ClientController::ClientController(const QString& databaseFilename, QObject *parent)
    : Controller(parent)
{
    setConnection(new Connection);
    db_ = new ClientSqliteDataStorage(databaseFilename, this);

    if (db_->isStoredDeviceCertificateEmpty()) {
        const auto deviceCert = Crypto::generateNewDeviceCertificate();
        setDeviceCertificate(deviceCert);
        setDeviceCertificateHash(qHash(deviceCert));
        db_->storeDeviceCertificate(deviceCert);
        setUserCertificate({});
        setUserCertificateHash(0);
        setUserId(0);
        db_->clearUserCertificate();
    } else {
        const auto deviceCert = db_->restoreDeviceCertificate();
        setDeviceCertificate(deviceCert);
        setDeviceCertificateHash(qHash(deviceCert));
        if (db_->isStoredUserIdEmpty()) {
            setUserId(0);
        } else {
            setUserId(db_->restoreUserId());
        }
    }
    wDebug << deviceCertificateHash() << deviceCertificate().left(10);
}

void ClientController::connectToServer(const QString &hostName, quint16 port) {
    if (connection_->state() == QAbstractSocket::UnconnectedState) {
        connection_->connectToHost(hostName, port);
    }
}

void ClientController::disconnectFromServer() {
    connection_->close();
}

void ClientController::sendHandshakeChallangeReply(const QString &reply) {
    connection_->send(CS_HANDSHAKE_SOLUTION{ reply.toUtf8() });
}

void ClientController::changeDeviceCertificate() {
    const auto deviceCert = Crypto::generateNewDeviceCertificate();
    setDeviceCertificate(deviceCert);
    setDeviceCertificateHash(qHash(deviceCert));
    wDebug << deviceCertificateHash() << deviceCertificate().left(10);
    db_->storeDeviceCertificate(deviceCert);
    setUserCertificate({});
    setUserCertificateHash(0);
    setUserId(0);
    db_->clearUserCertificate();
    disconnectFromServer();
}

void ClientController::createNewUser(const QByteArray& password) {
    const auto userCert = Crypto::generateNewUserCertificate();
    setUserCertificate( userCert );
    setUserCertificateHash(qHash(userCert));
    db_->storeUserCertificate(userCert);
    connection_->send(CS_NEW_USER{ userCert, password });
}

void ClientController::useOldUser(quint64 userId, const QString &password) {
    // TODO: tranfer user certificate from other device:
    // send userid and pass to server
    // server checks user id and pass
    // server sends challenge to other user devices and log
    // server sends challenge request to this client and awaits correct reply
    connection_->send(CS_OLD_USER{ userId, password });
}

void ClientController::addContact(quint64 userId) {
    // store contact as pending
    connection_->send(CC_ADD_CONTACT_REQUEST{ userId });
}

void ClientController::confirmAddingContact(quint64 userId) {
    // store contact as confirmed
    connection_->send(CC_ADD_CONTACT_ACCEPT{ userId });
}

void ClientController::sendMessage(quint64 userId, const QString &message) {
    // TODO: decrypt
    const auto encryptedMessage = message.toUtf8();
    connection_->send(CC_MESSAGE{ userId, encryptedMessage });
}

void ClientController::logout() {
    disconnectFromServer();
}

void ClientController::onPlainCommandReceived(SerializedCommand cmd) {
    switch (cmd.id_) {
        HANDLE_COMMAND(SC_HANDSHAKE_REPLY);
        HANDLE_COMMAND(SC_HANDSHAKE_RETRY);
        HANDLE_COMMAND(SC_HANDSHAKE_SUCCESSFULL);
        HANDLE_COMMAND(SC_NEW_USER_CREATED);
        HANDLE_COMMAND(SC_AUTHORIZED);
        HANDLE_COMMAND(CC_ADD_CONTACT_REQUEST);
        HANDLE_COMMAND(CC_ADD_CONTACT_REQUEST_COMPLETED);
        HANDLE_COMMAND(CC_ADD_CONTACT_ACCEPT);
        HANDLE_COMMAND(CC_MESSAGE);
        default: {
            wError << "unexpected command";
        }
    }
}

void ClientController::onEncryptedCommandReceived(EncryptedCommand cmd) {
    // TODO: decrypt and call onPlainCommandReceived
}

void ClientController::onConnectionStateChanged(QAbstractSocket::SocketState state) {
    setConnectionState(state);

    if (state == QAbstractSocket::ConnectedState) {
        connection_->send(CS_HANDSHAKE_REQUEST{ QByteArray(deviceCertificate()), 0, 0, 1 });
    } else if (state == QAbstractSocket::UnconnectedState) {
        // TODO: we did disconnect do a state reset
        setAuthorized(false);
    }
}

DEFINE_CLIENT_HANDLER(SC_HANDSHAKE_REPLY, cmd) {
    wDebug;
    if (!authorized()) {
        const auto c = cmd.deserialize<SC_HANDSHAKE_REPLY>();
        emit handshakeChallenge(c.handshakeReply);
    }
}

DEFINE_CLIENT_HANDLER(SC_HANDSHAKE_RETRY, cmd) {
    wDebug;
    if (!authorized()) {
        emit handshakeRetry();
    }
}

DEFINE_CLIENT_HANDLER(SC_HANDSHAKE_SUCCESSFULL, cmd) {
    wDebug;
    if (!authorized()) {
        const auto c = cmd.deserialize<SC_HANDSHAKE_SUCCESSFULL>();
        emit handshakeSuccessfull();
    }
}

DEFINE_CLIENT_HANDLER(SC_NEW_USER_CREATED, cmd) {
    wDebug;
    if (!authorized()) {
        const auto c = cmd.deserialize<SC_NEW_USER_CREATED>();
        setUserId(c.userId);
        db_->storeUserId(c.userId);
    }
}

DEFINE_CLIENT_HANDLER(SC_AUTHORIZED, cmd) {
    wDebug;
    if (!authorized()) {
        setAuthorized(true);
    }
}

DEFINE_CLIENT_HANDLER(CC_ADD_CONTACT_REQUEST, cmd) {
    wDebug;
    if (authorized()) {
        const auto c = cmd.deserialize<CC_ADD_CONTACT_REQUEST>();
        emit contactRequest(c.userId);
    }
}

DEFINE_CLIENT_HANDLER(CC_ADD_CONTACT_REQUEST_COMPLETED, cmd) {
    wDebug;
    if (authorized()) {
        const auto c = cmd.deserialize<CC_ADD_CONTACT_REQUEST_COMPLETED>();
        // TODO: notify that contact request is finished on different device
    }
}

DEFINE_CLIENT_HANDLER(CC_ADD_CONTACT_ACCEPT, cmd) {
    wDebug;
    if (authorized()) {
        const auto c = cmd.deserialize<CC_ADD_CONTACT_ACCEPT>();
        emit contactAccepted(c.userId);
    }
}

DEFINE_CLIENT_HANDLER(CC_MESSAGE, cmd) {
    wDebug;
    if (authorized()) {
        const auto c = cmd.deserialize<CC_MESSAGE>();
        // TODO: decrypt message
        emit incomingMessage(c.userId, c.encryptedMessage);
    }
}

QAbstractSocket::SocketState ClientController::connectionState() const {
    return connectionState_;
}

void ClientController::setConnectionState(QAbstractSocket::SocketState connectionState) {
    if (connectionState_ == connectionState) { return; }
    connectionState_ = connectionState;
    emit connectionStateChanged(connectionState_);
}

} // namespace client
} // namespace whisper


