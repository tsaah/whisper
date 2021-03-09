#include "mock_connection.h"

namespace whisper {
namespace network {
namespace test {

Mock_Connection::Mock_Connection(QObject* parent)
    : Connection(parent)
{
}

bool Mock_Connection::event(QEvent *event) {
    ++event_called_;
    event_event_ = event;
    return event_retern_;
}

bool Mock_Connection::eventFilter(QObject *watched, QEvent *event) {
    ++eventFilter_called_;
    eventFilter_watched_ = watched;
    eventFilter_event_ = event;
    return eventFilter_return_;
}

bool Mock_Connection::isSequential() const {
    ++isSequential_called_;
    return isSequential_return_;
}

bool Mock_Connection::open(QIODevice::OpenMode mode) {
    ++open_called_;
    open_mode_ = mode;
    return open_return_;
}

void Mock_Connection::close() {
    ++close_called_;
}

qint64 Mock_Connection::pos() const {
    ++pos_called_;
    return pos_return_;
}

qint64 Mock_Connection::size() const {
    ++size_called_;
    return size_return_;
}

bool Mock_Connection::seek(qint64 pos) {
    ++seek_called_;
    seek_pos_ = pos;
    return seek_return_;
}

bool Mock_Connection::atEnd() const {
    ++atEnd_called_;
    return atEnd_return_;
}

bool Mock_Connection::reset() {
    ++reset_called_;
    return reset_return_;
}

qint64 Mock_Connection::bytesAvailable() const {
    ++bytesAvailable_called_;
    return bytesAvailable_return_;
}

qint64 Mock_Connection::bytesToWrite() const {
    ++bytesToWrite_called_;
    return bytesToWrite_return_;
}

bool Mock_Connection::canReadLine() const {
    ++canReadLine_called_;
    return canReadLine_return_;
}

bool Mock_Connection::waitForReadyRead(int msecs) {
    ++waitForReadyRead_called_;
    waitForReadyRead_msecs_ = msecs;
    return waitForReadyRead_return_;
}

bool Mock_Connection::waitForBytesWritten(int msecs) {
    ++waitForBytesWritten_called_;
    waitForBytesWritten_msecs_ = msecs;
    return waitForBytesWritten_return_;
}

void Mock_Connection::resume() {
    ++resume_called_;
}

void Mock_Connection::connectToHost(const QString &hostName, quint16 port, QIODevice::OpenMode mode, QAbstractSocket::NetworkLayerProtocol protocol) {
    ++connectToHost_called_;
    connectToHost_hostName_ = hostName;
    connectToHost_port_ = port;
    connectToHost_mode_ = mode;
    connectToHost_protocol_ = protocol;
}

void Mock_Connection::connectToHost(const QHostAddress &address, quint16 port, QIODevice::OpenMode mode) {
    ++connectToHost_called_;
    connectToHost_address_ = address;
    connectToHost_port_ = port;
    connectToHost_mode_ = mode;
}

void Mock_Connection::disconnectFromHost() {
    ++disconnectFromHost_called_;
}

void Mock_Connection::setReadBufferSize(qint64 size) {
    ++setReadBufferSize_called_;
    setReadBufferSize_size_ = size;
}

qintptr Mock_Connection::socketDescriptor() const {
    ++socketDesctiptor_called_;
    return socketDescriptor_return_;
}

bool Mock_Connection::setSocketDescriptor(qintptr socketDescriptor, QAbstractSocket::SocketState state, QIODevice::OpenMode openMode) {
    ++setSocketDescriptor_called_;
    setSocketDescriptor_socketDescriptor_ = socketDescriptor;
    setSocketDescriptor_state_ = state;
    setSocketDescriptor_openMode_ = openMode;
    return setSocketDescriptor_return_;
}

void Mock_Connection::setSocketOption(QAbstractSocket::SocketOption option, const QVariant &value) {
    ++setSocketOption_called_;
    setSocketOption_option_ = option;
    setSocketOption_value_ = value;
}

QVariant Mock_Connection::socketOption(QAbstractSocket::SocketOption option) {
    ++socketOption_called_;
    socketOption_option_ = option;
    return socketOption_return_;
}

bool Mock_Connection::waitForConnected(int msecs) {
    ++waitForConnected_called_;
    waitForBytesWritten_msecs_ = msecs;
    return waitForConnected_return_;
}

bool Mock_Connection::waitForDisconnected(int msecs) {
    ++waitForDisconnected_called_;
    waitForDisconnected_msecs_ = msecs;
    return waitForDisconnected_return_;
}

void Mock_Connection::send(quint64 commandId, const QByteArray &command) {
    ++send_called_;
    send_commandId_ = commandId;
    send_command_ = command;
}

void Mock_Connection::emitAboutToClose() {
    emit aboutToClose();
}

void Mock_Connection::emitBytesWritten(qint64 bytes) {
    emit bytesWritten(bytes);
}

void Mock_Connection::emitChannelBytesWritten(int channel, qint64 bytes) {
    emit channelBytesWritten(channel, bytes);
}

void Mock_Connection::emitChannelReadyRead(int channel) {
    emit channelReadyRead(channel);
}

void Mock_Connection::emitReadChannelFinished() {
    emit readChannelFinished();
}

void Mock_Connection::emitReadyRead() {
    emit readyRead();
}

void Mock_Connection::emitConnected() {
    emit connected();
}

void Mock_Connection::emitDisconnected() {
    emit disconnected();
}

void Mock_Connection::emitErrorOccurred(QAbstractSocket::SocketError socketError) {
    emit errorOccurred(socketError);
}

void Mock_Connection::emitHostFound() {
    emit hostFound();
}

void Mock_Connection::emitProxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator) {
    emit proxyAuthenticationRequired(proxy, authenticator);
}

void Mock_Connection::emitStateChanged(QAbstractSocket::SocketState socketState) {
    emit stateChanged(socketState);
}

void Mock_Connection::emitEncrypted() {
    emit encrypted();
}

void Mock_Connection::emitEncryptedBytesWritten(qint64 written) {
    emit encryptedBytesWritten(written);
}

void Mock_Connection::emitModeChanged(QSslSocket::SslMode mode) {
    emit modeChanged(mode);
}

void Mock_Connection::emitNewSessionTicketReceived() {
    emit newSessionTicketReceived();
}

void Mock_Connection::emitPeerVerifyError(const QSslError &error) {
    emit peerVerifyError(error);
}

void Mock_Connection::emitPreSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *authenticator) {
    emit preSharedKeyAuthenticationRequired(authenticator);
}

void Mock_Connection::emitSslErrors(const QList<QSslError> &errors) {
    emit sslErrors(errors);
}

void Mock_Connection::emitPacketReceived(quint64 commandId, QByteArray command) {
    emit packetReceived(commandId, command);
}

} // namespace test
} // namespace network
} // namespace whisper
