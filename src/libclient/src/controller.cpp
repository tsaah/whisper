#include "controller.h"
#include "clientconnectionstate.h"
#include "clientcommandprocessor.h"
#include "clientsqlitedatastorage.h"

#include <connection.h>

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
    connect(connection_, &Connection::stateChanged, this, &Controller::onConnectionStateChanged);
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

void Controller::setConnectionState(int connectionState) {
    if (connectionState_ == connectionState) { return; }
    connectionState_ = connectionState;
    emit connectionStateChanged(connectionState_);
}

void Controller::onConnectionStateChanged(QAbstractSocket::SocketState state) {
    setConnectionState(static_cast<int>(state));
}

} // namespace client
} // namespace whisper
