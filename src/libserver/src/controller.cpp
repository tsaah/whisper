#include "controller.h"

#include "serverconnectionstate.h"
#include "servercommandprocessor.h"
#include "serversqlitedatastorage.h"

#include <connection.h>
#include <crypto.h>

using namespace whisper::common;

namespace whisper {
namespace server {

Controller::Controller(common::Connection* connection, QObject *parent)
    : QObject(parent)
    , connection_(connection)
{
    connection_->setParent(this);
    state_ = new ServerConnectionState(this);
    processor_ = new ServerCommandProcessor(this);
    data_ = new ServerSqliteDataStorage(this);

    connect(connection_, &Connection::commandReceived, this, &Controller::onCommandReceived);
    connect(connection_, &Connection::disconnected, this, &Controller::onDisconnected);
    connect(&killTimer_, &QTimer::timeout, connection_, &Connection::close);

    startKillTimer(1min);
}

void Controller::startKillTimer(std::chrono::milliseconds timer) {
    killTimer_.start(timer);
}

void Controller::stopKillTimer() {
    killTimer_.stop();
}

bool Controller::authorized() const {
    return authorized_;
}

quint64 Controller::userId() const {
    return userId_;
}

uint Controller::deviceHash() const {
    return deviceHash_;
}

void Controller::onCommandReceived(SerializedCommand command) {
    processor_->processCommand(connection_, command, state_, data_);
}

void Controller::onDisconnected() {
    emit controllerClosed();
}

void Controller::setAuthorized(bool authorized) {
    if (authorized_ == authorized) { return; }
    authorized_ = authorized;
    emit authorizedChanged(authorized_);
}

void Controller::setUserId(quint64 userId) {
    if (userId_ == userId) { return; }
    userId_ = userId;
    emit userIdChanged(userId_);
}

void Controller::setDeviceHash(uint deviceHash) {
    if (deviceHash_ == deviceHash) { return; }
    deviceHash_ = deviceHash;
    emit deviceHashChanged(deviceHash_);
}

} // namespace server
} // namespace whisper
