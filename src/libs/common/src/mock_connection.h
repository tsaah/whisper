#ifndef MOCK_SOCKET_H
#define MOCK_SOCKET_H

#include "common_export.h"
#include "connection.h"

#include <QHostAddress>

namespace whisper {
namespace network {
namespace test {

class WHISPER_COMMON_LIB Mock_Connection: public Connection {
    Q_OBJECT
public:
    Mock_Connection(QObject* parent = nullptr);

    // QObject interface
    bool event(QEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

    // QIODevice interface
    bool isSequential() const override;
    bool open(OpenMode mode) override;
    void close() override;
    qint64 pos() const override;
    qint64 size() const override;
    bool seek(qint64 pos) override;
    bool atEnd() const override;
    bool reset() override;
    qint64 bytesAvailable() const override;
    qint64 bytesToWrite() const override;
    bool canReadLine() const override;
    bool waitForReadyRead(int msecs) override;
    bool waitForBytesWritten(int msecs) override;

    // QAbstractSocket interface
    void resume() override;
    void connectToHost(const QString &hostName, quint16 port, OpenMode mode, NetworkLayerProtocol protocol) override;
    void connectToHost(const QHostAddress &address, quint16 port, OpenMode mode) override;
    void disconnectFromHost() override;
    void setReadBufferSize(qint64 size) override;
    qintptr socketDescriptor() const override;
    bool setSocketDescriptor(qintptr socketDescriptor, SocketState state, OpenMode openMode) override;
    void setSocketOption(QAbstractSocket::SocketOption option, const QVariant &value) override;
    QVariant socketOption(QAbstractSocket::SocketOption option) override;
    bool waitForConnected(int msecs) override;
    bool waitForDisconnected(int msecs) override;

    void send(quint64 commandId, const QByteArray& command) override;

    // signals
    void emitAboutToClose();
    void emitBytesWritten(qint64 bytes);
    void emitChannelBytesWritten(int channel, qint64 bytes);
    void emitChannelReadyRead(int channel);
    void emitReadChannelFinished();
    void emitReadyRead();

    void emitConnected();
    void emitDisconnected();
    void emitErrorOccurred(QAbstractSocket::SocketError socketError);
    void emitHostFound();
    void emitProxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator);
    void emitStateChanged(QAbstractSocket::SocketState socketState);

    void emitEncrypted();
    void emitEncryptedBytesWritten(qint64 written);
    void emitModeChanged(QSslSocket::SslMode mode);
    void emitNewSessionTicketReceived();
    void emitPeerVerifyError(const QSslError &error);
    void emitPreSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *authenticator);
    void emitSslErrors(const QList<QSslError> &errors);

    void emitPacketReceived(quint64 commandId, QByteArray command);

    int event_called_{ 0 };
    QEvent* event_event_{ nullptr };
    bool event_retern_{ false };

    int eventFilter_called_{ 0 };
    QObject* eventFilter_watched_{ nullptr };
    QEvent* eventFilter_event_{ nullptr };
    bool eventFilter_return_{ false };

    mutable int isSequential_called_{ 0 };
    bool isSequential_return_{ false };

    int open_called_{ 0 };
    OpenMode open_mode_{ QIODevice::NotOpen };
    bool open_return_{ false };

    int close_called_{ 0 };

    mutable int pos_called_{ 0 };
    qint64 pos_return_{ 0 };

    mutable int size_called_{ 0 };
    qint64 size_return_{ 0 };

    int seek_called_{ 0 };
    qint64 seek_pos_{ 0 };
    bool seek_return_{ 0 };

    mutable int atEnd_called_{ 0 };
    bool atEnd_return_{ false };

    int reset_called_{ 0 };
    bool reset_return_{ false };

    mutable int bytesAvailable_called_{ 0 };
    qint64 bytesAvailable_return_{ 0 };

    mutable int bytesToWrite_called_{ 0 };
    qint64 bytesToWrite_return_{ 0 };

    mutable int canReadLine_called_{ 0 };
    bool canReadLine_return_{ false };

    int waitForReadyRead_called_{ 0 };
    int waitForReadyRead_msecs_{ 0 };
    bool waitForReadyRead_return_{ false };

    int waitForBytesWritten_called_{ 0 };
    int waitForBytesWritten_msecs_{ 0 };
    bool waitForBytesWritten_return_{ false };

    int resume_called_{ 0 };

    int connectToHost_called_{ 0 };
    QString connectToHost_hostName_;
    QHostAddress connectToHost_address_;
    quint16 connectToHost_port_{ 0 };
    OpenMode connectToHost_mode_{ QIODevice::NotOpen };
    NetworkLayerProtocol connectToHost_protocol_{ QAbstractSocket::UnknownNetworkLayerProtocol };

    int disconnectFromHost_called_{ 0 };

    int setReadBufferSize_called_{ 0 };
    qint64 setReadBufferSize_size_{ 0 };

    mutable int socketDesctiptor_called_{ 0 };
    qintptr socketDescriptor_return_{ 0 };

    int setSocketDescriptor_called_{ 0 };
    qintptr setSocketDescriptor_socketDescriptor_{ 0 };
    SocketState setSocketDescriptor_state_{ QAbstractSocket::UnconnectedState };
    OpenMode setSocketDescriptor_openMode_{ QIODevice::NotOpen };
    bool setSocketDescriptor_return_{ false };

    int setSocketOption_called_{ 0 };
    QAbstractSocket::SocketOption setSocketOption_option_{ QAbstractSocket::LowDelayOption };
    QVariant setSocketOption_value_;

    int socketOption_called_{ 0 };
    QAbstractSocket::SocketOption socketOption_option_{ QAbstractSocket::LowDelayOption };
    QVariant socketOption_return_;

    int waitForConnected_called_{ 0 };
    int waitForConnected_msecs_{ 0 };
    bool waitForConnected_return_{ false };

    int waitForDisconnected_called_{ 0 };
    int waitForDisconnected_msecs_{ 0 };
    bool waitForDisconnected_return_{ false };

    int send_called_{ 0 };
    quint64 send_commandId_{ 0 };
    QByteArray send_command_;
};

} // namespace test
} // namespace network
} // namespace whisper

#endif // MOCK_SOCKET_H
