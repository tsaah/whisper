#ifndef WHISPER_CLIENT_CLIENTCONTROLLER_H
#define WHISPER_CLIENT_CLIENTCONTROLLER_H

#include "libclient_export.h"
#include "contactlistmodel.h"

#include <controller.h>

#include <QAbstractSocket>

namespace whisper {
namespace client {

class ClientSqliteDataStorage;

class WHISPER_LIBCLIENT ClientController: public common::Controller {
    Q_OBJECT
    Q_PROPERTY(QAbstractSocket::SocketState connectionState READ connectionState WRITE setConnectionState NOTIFY connectionStateChanged)
    Q_PROPERTY(ContactListModel* contactListModel READ contactListModel NOTIFY contactListModelChanged)
public:
    explicit ClientController(const QString& databaseFilename, QObject *parent = nullptr);

public slots:
    void connectToServer(const QString& hostName, quint16 port);
    void disconnectFromServer();
    void sendHandshakeChallangeReply(const QString& reply);
    void changeDeviceCertificate();
    void createNewUser(const QByteArray& password);
    void useOldUser(quint64 userId, const QString &password);
    void addContact(quint64 userId);
    void confirmAddingContact(quint64 userId);
    void sendMessage(quint64 userId, const QString& message);
    void logout();
    void requestAddContact(quint64 userId);

signals:
    void handshakeChallenge(QString challenge);
    void handshakeRetry();
    void handshakeSuccessfull();
    void contactRequest(quint64);
    void contactAccepted(quint64);
    void incomingMessage(quint64, QString);
    void controllerClosed();

protected slots:
    void onPlainCommandReceived(common::SerializedCommand cmd) override;
    void onEncryptedCommandReceived(common::EncryptedCommand cmd) override;
    void onConnectionStateChanged(QAbstractSocket::SocketState state) override;


    DECLARE_HANDLER(SC_HANDSHAKE_REPLY, cmd);
    DECLARE_HANDLER(SC_HANDSHAKE_RETRY, cmd);
    DECLARE_HANDLER(SC_HANDSHAKE_SUCCESSFULL, cmd);
    DECLARE_HANDLER(SC_NEW_USER_CREATED, cmd);
    DECLARE_HANDLER(SC_AUTHORIZED, cmd);
    DECLARE_HANDLER(CC_ADD_CONTACT_REQUEST, cmd);
    DECLARE_HANDLER(CC_ADD_CONTACT_REQUEST_COMPLETED, cmd);
    DECLARE_HANDLER(CC_ADD_CONTACT_ACCEPT, cmd);
    DECLARE_HANDLER(CC_MESSAGE, cmd);

private:
    ClientSqliteDataStorage* db_{ nullptr };





public:
    QAbstractSocket::SocketState connectionState() const;
    ContactListModel* contactListModel() const;

private slots:
    void setConnectionState(QAbstractSocket::SocketState connectionState);

signals:
    void connectionStateChanged(QAbstractSocket::SocketState connectionState);
    void contactListModelChanged(ContactListModel* contactListModel);

private:
    QAbstractSocket::SocketState connectionState_{ QAbstractSocket::UnconnectedState };
    ContactListModel* contactListModel_;
};

} // namespace client
} // namespace whisper

#define DEFINE_CLIENT_HANDLER(c, v) void ClientController::handle_##c(const SerializedCommand &v)

#endif // WHISPER_CLIENT_CLIENTCONTROLLER_H
