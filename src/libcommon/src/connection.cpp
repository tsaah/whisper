#include "connection.h"
#include "log.h"

#include <QDataStream>
#include <QDebug>

namespace whisper {
namespace common {

Connection::Connection(QObject* parent)
    : QSslSocket(parent)
{
    connect(this, &Connection::readyRead, this, &Connection::onReadyRead);
}

Connection::~Connection() {

}

void Connection::send(CommandId commandId, const Payload& payload) {
    const auto payloadSize = payload.size();
    PacketHeader h;
    h.magic = PacketHeader::s_magic;
    h.checksum = qChecksum(payload.constData(), payloadSize, PacketHeader::s_checksumType);
    h.commandId = commandId;
    h.version = PacketHeader::s_minimumVersion;

    bool compressed = false;
    Payload compressedPayload;
    if (payloadSize > PacketHeader::s_compressionBias) {
        h.flags = 1;
        compressedPayload = qCompress(payload, PacketHeader::s_compressionLevel);
        h.payloadSize = compressedPayload.size();
        compressed = true;
    } else {
        h.flags = 0;
        h.payloadSize = payloadSize;
    }
    const auto headWritten = write(reinterpret_cast<const char*>(&h), sizeof(PacketHeader));
    const auto payloadWritten = write(compressed ? compressedPayload : payload);

    wDebug << socketDescriptor() << "sent packet:" << commandId
           << "; headWritten:" << headWritten << "; payloadWritten:" << payloadWritten;
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
        if (packetHeader_.magic == 0) {
            const auto bytesToRead = bytesAvailable();
            if (bytesToRead < static_cast<qint64>(sizeof(PacketHeader))) {
                // too little data avaliable to even consider reading
                // we expect data to remeain in socket until we will read it next time
                // when more data will come
                wWarn << "too little data";
                return;
            }

            packetHeader_ = *reinterpret_cast<PacketHeader*>(read(sizeof(PacketHeader)).data());
            if (packetHeader_.magic != PacketHeader::s_magic) {
                wWarn << "incoming data had no correct magic";
                close();
                deleteLater();
                return;
            }

            if (packetHeader_.payloadSize < 0 || packetHeader_.payloadSize > PacketHeader::s_maximumPayloadSize) {
                wWarn << "incoming data had invalid payloadSize";
                close();
                deleteLater();
                return;
            }

            if (packetHeader_.version < PacketHeader::s_minimumVersion) {
                wWarn << "incoming packet version is too old to process";
                close();
                deleteLater();
                return;
            }
            // if there will be backward compatibility packet version support we should do it here

            // TODO: probably verify that command exists and could be received here
        }
        const auto bytesToReadAfterHeader = bytesAvailable();

        if (bytesToReadAfterHeader < packetHeader_.payloadSize) {
            return;
        }

        auto payload = read(packetHeader_.payloadSize);

        // m_packet.compressed = (m_packet.flags & 1) == 1; // for now 1 means compressed
        if (packetHeader_.flags & 1) {
            payload = qUncompress(payload);
        }

        const auto checksum = qChecksum(payload.constData(), payload.size(), PacketHeader::s_checksumType);

        if (packetHeader_.checksum == checksum) {
            wDebug << payload;
            emit packetReceived(packetHeader_.commandId, payload);
        } else {
            wWarn << "checksums didn't match";
            close();
            deleteLater();
            return;
        }

        packetHeader_ = {};

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

} // namespace common
} // namespace whisper
