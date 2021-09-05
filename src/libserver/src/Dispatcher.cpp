#include "ServerController.h"
#include "Dispatcher.h"
#include "ServerSqliteDataStorage.h"

#include <Connection.h>

using namespace whisper::common;

namespace whisper::server {

Dispatcher::Dispatcher(QObject *parent)
    : db_(new ServerSqliteDataStorage(this))
{
}

void Dispatcher::addConnection(Connection* connection) {
//    auto* controller = new ServerController(connection, db_, this, this);

//    connect(controller, &ServerController::authorizedChanged, this, &Dispatcher::onControllerAuthorized);

//    connect(controller, &ServerController::deviceCertificateHashChanged, this, &Dispatcher::onControllerDeviceChanged);

//    connect(controller, &ServerController::controllerClosed, this, &Dispatcher::onControllerClosed);

//    controllers_.insert(controller);
//    emit controllerAdded(controller);
}

void Dispatcher::enqueueContactRequest(quint64 userId) {
}

void Dispatcher::enqueueContactRequestAccepted(quint64 userId) {

}

void Dispatcher::enqueueMessage(quint64 userId, QByteArray encryptedMessage) {

}

void Dispatcher::onControllerAuthorized(quint64 userId) {
    auto* controller = qobject_cast<ServerController*>(sender());
    authorizedControllers_.insert(userId, controller);
}

void Dispatcher::onControllerDeviceChanged(uint deviceCertificateHash) {
    auto* controller = qobject_cast<ServerController*>(sender());
    controllersByDevice_.insert(deviceCertificateHash, controller);
}

void Dispatcher::onControllerClosed() {
//    auto* controller = qobject_cast<ServerController*>(sender());
//    authorizedControllers_.remove(controller->userId());
//    controllersByDevice_.remove(controller->deviceCertificateHash());
//    controllers_.remove(controller);
//    controller->deleteLater();
//    emit controllerRemoved(controller);
}

} // namespace whisper::server
