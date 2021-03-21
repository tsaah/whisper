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
        state_->deviceSertificate = data_->restoreDeviceCertificate();
    } else {
        const auto deviceCert = Crypto::generateNewDeviceCertificate();
        state_->deviceSertificate = deviceCert;
        data_->storeDeviceCertificate(deviceCert);
    }

    connect(processor_, &ClientCommandProcessor::handshakeChallenge, this, &Controller::handshakeChallenge);
    connect(processor_, &ClientCommandProcessor::handshakeRetry, this, &Controller::handshakeRetry);
    connect(processor_, &ClientCommandProcessor::handshakeSuccessfull, this, &Controller::handshakeSuccessfull);
}

int Controller::connectionState() const { return connectionState_; }

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
    state_->deviceSertificate = deviceCert;
    data_->storeDeviceCertificate(deviceCert);
}

void Controller::setConnectionState(int connectionState) {
    if (connectionState_ == connectionState) { return; }
    connectionState_ = connectionState;
    emit connectionStateChanged(connectionState_);
}

void Controller::onConnectionStateChanged(QAbstractSocket::SocketState state) {
    setConnectionState(static_cast<int>(state));
    if (state == QAbstractSocket::ConnectedState) {
        connection_->send(CS_HANDSHAKE_REQUEST{ state_->deviceSertificate, 0, 0, 1 });
    }
}

} // namespace client
} // namespace whisper
