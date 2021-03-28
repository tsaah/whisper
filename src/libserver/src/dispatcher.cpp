#include "controller.h"
#include "dispatcher.h"

#include <connection.h>

using namespace whisper::common;

namespace whisper {
namespace server {

Dispatcher::Dispatcher(QObject *parent) {

}

void Dispatcher::addConnection(Connection* connection) {
    auto* controller = new Controller(connection, this);

    connect(controller, &Controller::authorizedChanged, this, &Dispatcher::onControllerAuthorized);
    connect(controller, &Controller::deviceHashChanged, this, &Dispatcher::onControllerDeviceChanged);
    connect(controller, &Controller::controllerClosed, this, &Dispatcher::onControllerClosed);

    controllers_.insert(controller);
}

void Dispatcher::enqueueContactRequest(quint64 userId) {

}

void Dispatcher::enqueueContactRequestAccepted(quint64 userId) {

}

void Dispatcher::enqueueMessage(quint64 userId, QByteArray encryptedMessage) {

}

void Dispatcher::onControllerAuthorized(quint64 userId) {
    auto* controller = qobject_cast<Controller*>(sender());
    authorizedControllers_.insert(userId, controller);
}

void Dispatcher::onControllerDeviceChanged(uint deviceHash) {
    auto* controller = qobject_cast<Controller*>(sender());
    controllersByDevice_.insert(deviceHash, controller);
}

void Dispatcher::onControllerClosed() {
    auto* controller = qobject_cast<Controller*>(sender());
    authorizedControllers_.remove(controller->userId());
    controllersByDevice_.remove(controller->deviceHash());
    controllers_.remove(controller);
    controller->deleteLater();
}

} // namespace server
} // namespace whisper
