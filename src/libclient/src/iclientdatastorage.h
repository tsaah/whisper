#ifndef ICLIENTDATASTORAGE_H
#define ICLIENTDATASTORAGE_H

#include "libclient_export.h"

#include <datastorage.h>

#include <QSqlDatabase>

namespace whisper {
namespace client {

class WHISPER_LIBCLIENT IClientDataStorage: public common::DataStorage {
    Q_OBJECT
public:
    explicit IClientDataStorage(QObject* parent = nullptr): common::DataStorage(parent) {}

    virtual bool isStoredDeviceCertificateEmpty() const = 0;
    virtual QByteArray restoreDeviceCertificate() const = 0;
    virtual void storeDeviceCertificate(const QByteArray& deviceCertificate) = 0;
    virtual void clearDeviceCertificate() = 0;

    virtual bool isStoredUserCertificateEmpty() const = 0;
    virtual QByteArray restoreUserCertificate() const = 0;
    virtual void storeUserCertificate(const QByteArray &userCertificate) = 0;
    virtual void clearUserCertificate() = 0;

    virtual bool isStoredUserIdEmpty() const = 0;
    virtual quint64 restoreUserId() const = 0;
    virtual void storeUserId(quint64 userId) = 0;
    virtual void clearUserId() = 0;


};

} // namespace client
} // namespace whisper

#endif // ICLIENTDATASTORAGE_H
