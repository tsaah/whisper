#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "libserver_export.h"

#include <packet.h>

#include <QObject>
#include <QTimer>

#include <chrono>

using namespace std::chrono_literals;

namespace whisper {

namespace common {

class Connection;

} // namespace common

namespace server {

class ServerConnectionState;
class ServerCommandProcessor;
class ServerSqliteDataStorage;

class WHISPER_LIBSERVER Controller: public QObject {
    Q_OBJECT
    Q_PROPERTY(bool authorized READ authorized WRITE setAuthorized NOTIFY authorizedChanged)
    Q_PROPERTY(quint64 userId READ userId WRITE setUserId NOTIFY userIdChanged)
    Q_PROPERTY(uint deviceHash READ deviceHash WRITE setDeviceHash NOTIFY deviceHashChanged)
public:
    explicit Controller(common::Connection* connection, QObject* parent = nullptr);

    bool authorized() const;
    quint64 userId() const;
    uint deviceHash() const;

private slots:
    void onCommandReceived(common::SerializedCommand command);
    void onDisconnected();
    void setAuthorized(bool authorized);
    void setUserId(quint64 userId);
    void setDeviceHash(uint deviceHash);
    void startKillTimer(std::chrono::milliseconds timer);
    void stopKillTimer();

signals:
    void authorizedChanged(bool authorized);
    void userIdChanged(quint64 userId);
    void deviceHashChanged(uint deviceHash);
    void controllerClosed();

private:
    common::Connection* connection_{ nullptr };
    ServerConnectionState* state_{ nullptr };
    ServerCommandProcessor* processor_{ nullptr };
    ServerSqliteDataStorage* data_{ nullptr };
    QTimer killTimer_;
    bool authorized_{ false };
    quint64 userId_{ 0 };
    uint deviceHash_{ 0 };
};

} // namespace server
} // namespace whisper

#endif // CONTROLLER_H
