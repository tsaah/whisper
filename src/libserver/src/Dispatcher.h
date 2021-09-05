#pragma once

#include "libserver_export.h"

#include <Crypto/Crypto.h>

#include <QHash>
#include <QSet>
#include <QObject>

namespace whisper::common {

class Connection;

} // namespace whisper::common

namespace whisper::server {

class ServerController;
class ServerSqliteDataStorage;

class WHISPER_LIBSERVER Dispatcher: public QObject {
    Q_OBJECT
public:
    explicit Dispatcher(QObject *parent = nullptr);
    void addConnection(common::Connection* connection);

    void enqueueContactRequest(quint64 userId);
    void enqueueContactRequestAccepted(quint64 userId);
    void enqueueMessage(quint64 userId, QByteArray encryptedMessage);

signals:
    void controllerAdded(ServerController*);
    void controllerRemoved(ServerController*);

private slots:
    void onControllerAuthorized(quint64 userId);
    void onControllerDeviceChanged(uint deviceCertificateHash);
    void onControllerClosed();

private:
    QHash<uint, ServerController*> controllersByDevice_;
    QHash<quint64, ServerController*> authorizedControllers_;
    QSet<ServerController*> controllers_;
    ServerSqliteDataStorage* db_{ nullptr };
};

} // namespace whisper::server
