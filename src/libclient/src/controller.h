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
    Q_PROPERTY(quint64 userId READ userId WRITE setUserId NOTIFY userIdChanged)
    Q_PROPERTY(bool authorized READ authorized WRITE setAuthorized NOTIFY authorizedChanged)
public:
    explicit Controller(QObject *parent = nullptr);

    int connectionState() const;
    quint64 userId() const;
    bool authorized() const;

public slots:
    void connectToServer(const QString& hostName, quint16 port);
    void disconnectFromServer();
    void sendHandshakeChallangeReply(const QString& reply);
    void changeDeviceCertificate();
    void createNewUser(const QByteArray& password);
    void useOldUser(quint64 userId, const QByteArray& password);
    void addContact(quint64 userId);
    void confirmAddingContact(quint64 userId);
    void sendMessage(quint64 userId, const QString& message);

signals:
    void connectionStateChanged(int connectionState);
    void handshakeChallenge(QString challenge);
    void handshakeRetry();
    void handshakeSuccessfull();
    void userIdChanged(quint64 userId);
    void authorizedChanged(bool authorized);
    void contactRequest(quint64);
    void contactAccepted(quint64);
    void incomingMessage(quint64, QString);

private slots:
    void setUserId(quint64 userId);
    void setAuthorized(bool authorized);
    void setConnectionState(int connectionState);
    void onConnectionStateChanged(QAbstractSocket::SocketState state);
    void onIncomingMessage(quint64 userId, QByteArray encryptedMessage);

private:
    common::Connection* connection_{ nullptr };
    ClientConnectionState* state_{ nullptr };
    ClientCommandProcessor* processor_{ nullptr };
    ClientSqliteDataStorage* data_{ nullptr };

    int connectionState_{ 0 };
    quint64 userId_{ 0 };
    bool authorized_;
};

} // namespace client
} // namespace whisper

#endif // CONTROLLER_H
