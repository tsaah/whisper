#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "libclient_export.h"

#include <QAbstractSocket>
#include <QObject>

namespace whisper {

namespace common {

class Connection;

} // namespace common

namespace client {

class ClientConnectionState;
class ClientCommandProcessor;
class ClientSqliteDataStorage;

class WHISPER_LIBCLIENT Controller : public QObject {
    Q_OBJECT

    Q_PROPERTY(int connectionState READ connectionState WRITE setConnectionState NOTIFY connectionStateChanged)
public:
    explicit Controller(QObject *parent = nullptr);

    int connectionState() const;

public slots:
    void connectToServer(const QString& hostName, quint16 port);
    void disconnectFromServer();
    void sendHandshakeChallangeReply(const QString& reply);

signals:
    void connectionStateChanged(int connectionState);
    void handshakeChallenge(QString challenge);
    void handshakeRetry();

private slots:
    void setConnectionState(int connectionState);
    void onConnectionStateChanged(QAbstractSocket::SocketState state);

private:
    common::Connection* connection_{ nullptr };
    ClientConnectionState* state_{ nullptr };
    ClientCommandProcessor* processor_{ nullptr };
    ClientSqliteDataStorage* data_{ nullptr };

    int connectionState_;
};

} // namespace client
} // namespace whisper

#endif // CONTROLLER_H
