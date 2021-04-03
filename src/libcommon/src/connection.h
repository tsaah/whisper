#ifndef SOCKET_H
#define SOCKET_H

#include "libcommon_export.h"
#include "types.h"
#include "packet.h"

#include <QSslSocket>

namespace whisper {
namespace common {

class WHISPER_LIBCOMMON Connection: public QSslSocket {
    Q_OBJECT
public:
    explicit Connection(QObject* parent = nullptr);
    ~Connection() override;

public slots:
    void send(const SerializedCommand& command);
    void sendEncrypted(const EncryptedCommand& command);

signals:
    void plainCommandReceived(SerializedCommand);
    void encryptedCommandReceived(EncryptedCommand);

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
    #pragma pack(push, 1)
    struct PacketHeader {
        Magic magic{ 0 };
        Size payloadSize{ 0 };
        CommandId commandId{ 0 };
        Version version{ 0 };
        Flags flags{ 0 };
        Checksum checksum{ 0 };
        static constexpr Magic s_magic{ 0x4f3d3 };
        static constexpr Size s_maximumPayloadSize{ 2 * 1024 * 1024 }; // maximum 2 megabytes for now
        static constexpr Version s_minimumVersion{ 1 };
        static constexpr auto s_checksumType{ Qt::ChecksumItuV41 };
        static constexpr auto s_compressionBias{ 100 };
        static constexpr auto s_compressionLevel{ 9 };
    } packetHeader_;
    #pragma pack(pop)

    void send(CommandId commandId, const QByteArray& payload, bool encrypted);
};

} // namespace common
} // namespace whisper

#endif // SOCKET_H
