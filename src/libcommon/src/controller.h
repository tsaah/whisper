#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "libcommon_export.h"
#include "packet.h"
#include "crypto.h"

#include <QAbstractSocket>
#include <QObject>

namespace whisper {
namespace common {

class Connection;

class WHISPER_LIBCOMMON Controller: public QObject {
    Q_OBJECT
    Q_PROPERTY(Certificate deviceCertificate READ deviceCertificate WRITE setDeviceCertificate NOTIFY deviceCertificateChanged)
    Q_PROPERTY(uint deviceCertificateHash READ deviceCertificateHash WRITE setDeviceCertificateHash NOTIFY deviceCertificateHashChanged)
    Q_PROPERTY(Certificate userCertificate READ userCertificate WRITE setUserCertificate NOTIFY userCertificateChanged)
    Q_PROPERTY(uint userCertificateHash READ userCertificateHash WRITE setUserCertificateHash NOTIFY userCertificateHashChanged)
    Q_PROPERTY(quint64 userId READ userId WRITE setUserId NOTIFY userIdChanged)
    Q_PROPERTY(bool authorized READ authorized WRITE setAuthorized NOTIFY authorizedChanged)
public:
    explicit Controller(QObject* parent = nullptr);
    virtual ~Controller() = default;

    void setConnection(Connection* connection);

protected slots:
    virtual void onPlainCommandReceived(SerializedCommand) = 0;
    virtual void onEncryptedCommandReceived(EncryptedCommand) = 0;
    virtual void onConnectionStateChanged(QAbstractSocket::SocketState) = 0;

protected:
    Connection* connection_{ nullptr };



public:
    Certificate deviceCertificate() const;
    uint deviceCertificateHash() const;
    Certificate userCertificate() const;
    uint userCertificateHash() const;
    quint64 userId() const;
    bool authorized() const;

public slots:
    void setDeviceCertificate(Certificate deviceCertificate);
    void setDeviceCertificateHash(uint deviceCertificateHash);
    void setUserCertificate(Certificate userCertificate);
    void setUserCertificateHash(uint userCertificateHash);
    void setUserId(quint64 userId);
    void setAuthorized(bool authorized);

signals:
    void deviceCertificateChanged(Certificate deviceCertificate);
    void deviceCertificateHashChanged(uint deviceCertificateHash);
    void userCertificateChanged(Certificate userCertificate);
    void userCertificateHashChanged(uint userCertificateHash);
    void userIdChanged(quint64 userId);
    void authorizedChanged(bool authorized);

private:
    Certificate deviceCertificate_;
    uint deviceCertificateHash_{ 0 };
    Certificate userCertificate_;
    uint userCertificateHash_{ 0 };
    quint64 userId_{ 0 };
    bool authorized_{ false };
};

} // namespace common
} // namespace whisper

#endif // CONTROLLER_H
