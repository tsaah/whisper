#include "connection.h"
#include "log.h"

#include <QDataStream>
#include <QDebug>

namespace whisper {
namespace network {

Connection::Connection(QObject* parent)
    : QSslSocket(parent)
{
    connect(this, &Connection::readyRead, this, &Connection::onReadyRead);
}

Connection::~Connection() {

}

void Connection::send(quint64 commandId, const QByteArray &payload) {
    QDataStream stream(this);
    stream.setVersion(QDataStream::Qt_5_15);
    stream << PacketHeader::s_magic << commandId << static_cast<quint64>(payload.size()) << payload;
}

void Connection::onAboutToClose() {
    wDebug;
}

void Connection::onBytesWritten(qint64 bytes) {
    wDebug;
}

void Connection::onChannelBytesWritten(int channel, qint64 bytes) {
    wDebug;
}

void Connection::onChannelReadyRead(int channel) {
    wDebug;
}

void Connection::onReadChannelFinished() {
    wDebug;
}

void Connection::onReadyRead() {
    wDebug << socketDescriptor();

    if (bytesAvailable() <= 0) {
        wDebug << "some faulty shit is happening";
        close();
        deleteLater();
        return;
    }

    do {
        if (currentPacket_.magic == 0 && static_cast<quint64>(bytesAvailable()) >= sizeof(PacketHeader::magic)) {
            currentPacket_.magic = read(sizeof(PacketHeader::magic)).toULongLong();
            if (currentPacket_.magic != PacketHeader::s_magic) {
                wDebug << "wrong magic";
                close();
                deleteLater();
                return;
            }
        }
        if (currentPacket_.commandId == 0 && static_cast<quint64>(bytesAvailable()) >= sizeof(PacketHeader::commandId)) {
            stream >> currentPacket_.commandId;
        }
        if (currentPacket_.size == 0 && static_cast<quint64>(bytesAvailable())>= sizeof(PacketHeader::size)) {
            stream >> currentPacket_.size;
        }
        if (currentPacket_.payload.isEmpty() && static_cast<quint64>(bytesAvailable()) >= currentPacket_.size) {
            stream >> currentPacket_.payload;
            wDebug << currentPacket_.payload;
            emit packetReceived(currentPacket_.commandId, currentPacket_.payload);
            currentPacket_ = {};
            if (bytesAvailable() > 0) {
                emit readyRead();
            }
        }
    } while (static_cast<quint64>(bytesAvailable()) > sizeof(PacketHeader::magic));



}

void Connection::onConnected() {
    wDebug;
}

void Connection::onDisconnected() {
    wDebug;
}

void Connection::onErrorOccurred(QAbstractSocket::SocketError socketError) {
    wDebug;
}

void Connection::onHostFound() {
    wDebug;
}

void Connection::onProxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator) {
    wDebug;
}

void Connection::onStateChanged(QAbstractSocket::SocketState socketState) {
    wDebug;
}

void Connection::onEncrypted() {
    wDebug;
}

void Connection::onEncryptedBytesWritten(qint64 written) {
    wDebug;
}

void Connection::onModeChanged(QSslSocket::SslMode mode) {
    wDebug;
}

void Connection::onNewSessionTicketReceived() {
    wDebug;
}

void Connection::onPeerVerifyError(const QSslError &error) {
    wDebug;
}

void Connection::onPreSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *authenticator) {
    wDebug;
}

void Connection::onSslErrors(const QList<QSslError> &errors) {
    wDebug;
}

} // namespace network
} // namespace whisper
