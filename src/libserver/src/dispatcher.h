#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "libserver_export.h"

#include <QHash>
#include <QObject>

namespace whisper {

namespace common {

class Connection;

} // namespace common

namespace server {

class Controller;

class WHISPER_LIBSERVER Dispatcher: public QObject {
    Q_OBJECT
public:
    explicit Dispatcher(QObject *parent = nullptr);
    void addConnection(common::Connection* connection);

    void enqueueContactRequest(quint64 userId);
    void enqueueContactRequestAccepted(quint64 userId);
    void enqueueMessage(quint64 userId, QByteArray encryptedMessage);

signals:

private slots:
    void onControllerAuthorized(quint64 userId);
    void onControllerDeviceChanged(uint deviceHash);
    void onControllerClosed();

private:
    QHash<uint, Controller*> controllersByDevice_;
    QHash<quint64, Controller*> authorizedControllers_;
    QSet<Controller*> controllers_;
};

} // namespace server
} // namespace whisper

#endif // DISPATCHER_H
