#include "log.h"

#include "Connection.h"

#include <QDataStream>
#include <QDebug>

namespace whisper::common {

Connection::Connection(QObject* parent)
    : QSslSocket(parent)
{
    connect(this, &Connection::readyRead, this, &Connection::onReadyRead);
}

Connection::~Connection() {
    wDebug << "connection destroyed";
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
    if (bytesAvailable() <= 0) {
        wDebug << "some faulty shit is happening";
        packetHeader_ = {};
        close();
        return;
    }

    do {
        if (packetHeader_.magic == 0) {
            const auto bytesToRead = bytesAvailable();
            if (bytesToRead < static_cast<qint64>(sizeof(PacketHeader))) {
                // too little data avaliable to even consider reading
                // we expect data to remeain in socket until we will read it next time
                // when more data will come
                packetHeader_ = {};
                return;
            }

            const auto headerBytesRead = read(reinterpret_cast<char*>(&packetHeader_), sizeof(PacketHeader));
            if (headerBytesRead != sizeof(PacketHeader) || packetHeader_.magic != PacketHeader::s_magic) {
                wWarn << "incoming data had no correct magic";
                packetHeader_ = {};
                close();
                return;
            }

            if (packetHeader_.payloadSize < 0 || packetHeader_.payloadSize > PacketHeader::s_maximumPayloadSize) {
                wWarn << "incoming data had invalid payloadSize";
                packetHeader_ = {};
                close();
                return;
            }

            if (packetHeader_.version < PacketHeader::s_minimumVersion) {
                wWarn << "incoming request version is too old to process";
                packetHeader_ = {};
                close();
                return;
            }

            if (packetHeader_.requestType == request::RequestType::Type::Unknown) {
                wWarn << "incoming request type is is unknown";
                packetHeader_ = {};
                close();
                return;
            }

            // if there will be backward compatibility packet version support we should do it here
        }
        const auto bytesToReadAfterHeader = bytesAvailable();

        if (bytesToReadAfterHeader < packetHeader_.payloadSize) {
            return;
        }

        auto payload = read(packetHeader_.payloadSize);

        if ((packetHeader_.flags & 1) != 0) {
            payload = qUncompress(payload);
        }

        const auto checksum = qChecksum(payload.constData(), payload.size(), PacketHeader::s_checksumType);

        if (packetHeader_.checksum == checksum) {
            emit requestRecieved(packetHeader_.requestType, payload);
        } else {
            wWarn << "checksums didn't match";
            packetHeader_ = {};
            close();
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

void Connection::send(request::RequestBasePtr request) {
    const auto payload = request->serialize();
    const auto payloadSize = payload.size();
    PacketHeader h;
    h.magic = PacketHeader::s_magic;
    h.checksum = qChecksum(payload.constData(), payloadSize, PacketHeader::s_checksumType);
    h.version = PacketHeader::s_minimumVersion;
    h.requestType = request->requestType();
    h.flags = 0;

    bool compressed = false;
    QByteArray compressedPayload;
    if (payloadSize > PacketHeader::s_compressionBias) {
        h.flags |= 1;
        compressedPayload = qCompress(payload, PacketHeader::s_compressionLevel);
        h.payloadSize = compressedPayload.size();
        compressed = true;
    } else {
        h.payloadSize = payloadSize;
    }

    const auto headWritten = write(reinterpret_cast<const char*>(&h), sizeof(PacketHeader));
    const auto payloadWritten = write(compressed ? compressedPayload : payload);

    wDebug << socketDescriptor() << "sent data:" << " headWritten:" << headWritten << "; payloadWritten:" << payloadWritten;
}

} // namespace whisper::common
