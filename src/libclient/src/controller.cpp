#include "controller.h"
#include "clientconnectionstate.h"
#include "clientcommandprocessor.h"
#include "clientsqlitedatastorage.h"

#include <connection.h>
#include <crypto.h>

using namespace whisper::common;

namespace whisper {
namespace client {

Controller::Controller(QObject *parent)
    : QObject(parent)
{
    connection_ = new Connection(this);
    state_ = new ClientConnectionState(connection_);
    processor_ = new ClientCommandProcessor(connection_);
    data_ = new ClientSqliteDataStorage(connection_);
    connect(connection_, &Connection::commandReceived, [this](SerializedCommand command){
        processor_->processCommand(connection_, command, state_, data_);
    });
    connect(connection_, &Connection::disconnected, [this]{ processor_->reset(); });
    connect(connection_, &Connection::stateChanged, this, &Controller::onConnectionStateChanged);

    if (!data_->isStoredDeviceCertificateEmpty()) {
        state_->deviceCertificate = data_->restoreDeviceCertificate();
    } else {
        const auto deviceCert = Crypto::generateNewDeviceCertificate();
        state_->deviceCertificate = deviceCert;
        state_->userCertificate = {};
        data_->storeDeviceCertificate(deviceCert);
        data_->clearUserCertificate();
    }

    if (!data_->isStoredUserIdEmpty()) {
        setUserId(data_->restoreUserId());
    }

    connect(processor_, &ClientCommandProcessor::handshakeChallenge, this, &Controller::handshakeChallenge);
    connect(processor_, &ClientCommandProcessor::handshakeRetry, this, &Controller::handshakeRetry);
    connect(processor_, &ClientCommandProcessor::handshakeSuccessfull, this, &Controller::handshakeSuccessfull);
    connect(processor_, &ClientCommandProcessor::authorized, this, &Controller::setAuthorized);
    connect(processor_, &ClientCommandProcessor::newUserId, this, &Controller::setUserId);
    connect(processor_, &ClientCommandProcessor::contactRequest, this, &Controller::contactRequest);
    connect(processor_, &ClientCommandProcessor::contactAccepted, this, &Controller::contactAccepted);
    connect(processor_, &ClientCommandProcessor::incomingMessage, this, &Controller::onIncomingMessage);
}

int Controller::connectionState() const { return connectionState_; }

quint64 Controller::userId() const {
    return userId_;
}

bool Controller::authorized() const {
    return authorized_;
}

void Controller::connectToServer(const QString &hostName, quint16 port) {
    if (connection_->state() == QAbstractSocket::UnconnectedState) {
        connection_->connectToHost(hostName, port);
    }
}

void Controller::disconnectFromServer() {
    connection_->close();
}

void Controller::sendHandshakeChallangeReply(const QString &reply) {
    connection_->send(CS_HANDSHAKE_SOLUTION{reply.toUtf8()});
}

void Controller::changeDeviceCertificate() {
    disconnectFromServer();
    const auto deviceCert = Crypto::generateNewDeviceCertificate();
    state_->deviceCertificate = deviceCert;
    data_->storeDeviceCertificate(deviceCert);
}

void Controller::createNewUser(const QByteArray& password) {
    const auto userCert = Crypto::generateNewUserCertificate();
    state_->userCertificate = userCert;
    data_->storeUserCertificate(userCert);
    connection_->send(CS_NEW_USER{ userCert, password });
}

void Controller::useOldUser(quint64 userId, const QByteArray &password) {
    // send userid and pass to server
    // server checks user id and pass
    // server sends challenge to other user devices and log
    // server sends challenge request to this client and awaits correct reply
    // CONSIDER: using device certificate to encrypt messages or tranfer user certificate from other device?
}

void Controller::addContact(quint64 userId) {
    // store contact as pending
    connection_->send(CC_ADD_CONTACT_REQUEST{ userId });
}

void Controller::confirmAddingContact(quint64 userId) {
    // store contact as confirmed
    connection_->send(CC_ADD_CONTACT_ACCEPT{ userId });
}

void Controller::sendMessage(quint64 userId, const QString &message) {
    // TODO: decrypt
    const auto encryptedMessage = message.toUtf8();
    connection_->send(CC_MESSAGE{ userId, encryptedMessage });
}

void Controller::setUserId(quint64 userId) {
    if (userId_ == userId) { return; }
    userId_ = userId;
    emit userIdChanged(userId_);
}

void Controller::setAuthorized(bool authorized) {
    if (authorized_ == authorized) { return; }
    authorized_ = authorized;
    emit authorizedChanged(authorized_);
}

void Controller::setConnectionState(int connectionState) {
    if (connectionState_ == connectionState) { return; }
    connectionState_ = connectionState;
    emit connectionStateChanged(connectionState_);
}

void Controller::onConnectionStateChanged(QAbstractSocket::SocketState state) {
    setConnectionState(static_cast<int>(state));
    if (state == QAbstractSocket::ConnectedState) {
        connection_->send(CS_HANDSHAKE_REQUEST{ state_->deviceCertificate, 0, 0, 1 });
    }
}

void Controller::onIncomingMessage(quint64 userId, QByteArray encryptedMessage) {
    // TODO: decrypt
    const QString decryptedMessage = encryptedMessage;
    emit incomingMessage(userId, decryptedMessage);
}

} // namespace client
} // namespace whisper
