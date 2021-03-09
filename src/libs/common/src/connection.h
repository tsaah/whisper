#ifndef SOCKET_H
#define SOCKET_H

#include "common_export.h"

#include <QSslSocket>

namespace whisper {
namespace network {

class WHISPER_COMMON_LIB Connection: public QSslSocket {
    Q_OBJECT
public:
    explicit Connection(QObject* parent = nullptr);
    ~Connection();

    virtual void send(quint64 commandId, const QByteArray& payload);

signals:
    void packetReceived(quint64, QByteArray);

private slots:
    void onAboutToClose();
    void onBytesWritten(qint64 bytes);
    void onChannelBytesWritten(int channel, qint64 bytes);
    void onChannelReadyRead(int channel);
    void onReadChannelFinished();
    void onReadyRead();

    void onConnected();
    void onDisconnected();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);
    void onHostFound();
    void onProxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator);
    void onStateChanged(QAbstractSocket::SocketState socketState);

    void onEncrypted();
    void onEncryptedBytesWritten(qint64 written);
    void onModeChanged(QSslSocket::SslMode mode);
    void onNewSessionTicketReceived();
    void onPeerVerifyError(const QSslError &error);
    void onPreSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *authenticator);
    void onSslErrors(const QList<QSslError> &errors);

private:
    struct PacketHeader {
        quint64 magic{ 0 };
        quint64 commandId{ 0 };
        quint64 size{ 0 };
        QByteArray payload;
        static const quint64 s_magic{ 0x4f3d3e2e9adbc };
    } currentPacket_;
};

} // namespace network
} // namespace whisper

#endif // SOCKET_H
