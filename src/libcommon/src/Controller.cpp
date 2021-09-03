#include "Controller.h"
#include "Connection.h"

namespace whisper::common {

Controller::Controller(QObject* parent)
    : QObject(parent)
{
}

void Controller::setConnection(Connection *connection) {
    connection_ = connection;
    connection_->setParent(this);
//    connect(connection_, &Connection::plainCommandReceived, this, &Controller::onPlainCommandReceived);
//    connect(connection_, &Connection::encryptedCommandReceived, this, &Controller::onEncryptedCommandReceived);
    connect(connection_, &Connection::stateChanged, this, &Controller::onConnectionStateChanged);
}

//Certificate Controller::deviceCertificate() const {
//    return deviceCertificate_;
//}

//uint Controller::deviceCertificateHash() const {
//    return deviceCertificateHash_;
//}

//Certificate Controller::userCertificate() const {
//    return userCertificate_;
//}

//uint Controller::userCertificateHash() const {
//    return userCertificateHash_;
//}

quint64 Controller::userId() const {
    return userId_;
}

bool Controller::authorized() const {
    return authorized_;
}

//void Controller::setDeviceCertificate(Certificate deviceCertificate) {
//    if (deviceCertificate_ == deviceCertificate) { return; }
//    deviceCertificate_ = deviceCertificate;
//    emit deviceCertificateChanged(deviceCertificate_);
//}

//void Controller::setDeviceCertificateHash(uint deviceCertificateHash) {
//    if (deviceCertificateHash_ == deviceCertificateHash) { return; }
//    deviceCertificateHash_ = deviceCertificateHash;
//    emit deviceCertificateHashChanged(deviceCertificateHash_);
//}

//void Controller::setUserCertificate(Certificate userCertificate) {
//    if (userCertificate_ == userCertificate) { return; }
//    userCertificate_ = userCertificate;
//    emit userCertificateChanged(userCertificate_);
//}

//void Controller::setUserCertificateHash(uint userCertificateHash) {
//    if (userCertificateHash_ == userCertificateHash) { return; }
//    userCertificateHash_ = userCertificateHash;
//    emit userCertificateHashChanged(userCertificateHash_);
//}

void Controller::setUserId(quint64 userId) {
    if (userId_ == userId) { return; }
    userId_ = userId;
    emit userIdChanged(userId_);
}

void Controller::setAuthorized(bool authorized) {
    if (authorized_ == authorized) { return; }
    authorized_ = authorized;
    emit authorizedChanged(authorized_);
}

} // namespace whisper::common
