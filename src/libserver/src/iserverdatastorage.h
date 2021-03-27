#ifndef ISERVERDATASTORAGE_H
#define ISERVERDATASTORAGE_H

#include "libserver_export.h"

#include <datastorage.h>

#include <QSqlDatabase>

namespace whisper {
namespace server {

class WHISPER_LIBSERVER IServerDataStorage: public common::DataStorage {
    Q_OBJECT
public:
    explicit IServerDataStorage(QObject* parent = nullptr): common::DataStorage(parent) {}

    virtual bool deviceExists(const QByteArray& deviceCert) const = 0;
    virtual void rememberDevice(const QByteArray& deviceCert) = 0;

    virtual bool userExists(const QByteArray& userCert) const = 0;
    virtual bool userIdExists(quint64 userId) const = 0;
    virtual void rememberNewUser(quint64 userId, const QByteArray& userCert, const QByteArray& deviceCert, const QByteArray& passwordHash, const QByteArray &passwordSalt) = 0;
    virtual QByteArray getUserCertificate(quint64 userId) const = 0;
    virtual QByteArray getPasswordHash(quint64 userId) const = 0;
    virtual QByteArray getPasswordSalt(quint64 userId) const = 0;

};

} // namespace server
} // namespace whisper

#endif // ISERVERDATASTORAGE_H
