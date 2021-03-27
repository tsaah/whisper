#ifndef SERVERSQLITEDATASTORAGE_H
#define SERVERSQLITEDATASTORAGE_H

#include "iserverdatastorage.h"

namespace whisper {
namespace server {

class WHISPER_LIBSERVER ServerSqliteDataStorage: public IServerDataStorage {
    Q_OBJECT
public:
    explicit ServerSqliteDataStorage(QObject* parent = nullptr);

    bool deviceExists(const QByteArray& deviceCert) const override;
    void rememberDevice(const QByteArray& deviceCert) override;

    bool userExists(const QByteArray& deviceCert) const override;
    bool userIdExists(quint64 userId) const override;
    void rememberNewUser(quint64 userId, const QByteArray& userCert, const QByteArray& deviceCert, const QByteArray& passwordHash, const QByteArray &passwordSalt) override;
    QByteArray getUserCertificate(quint64 userId) const override;
    QByteArray getPasswordHash(quint64 userId) const override;
    QByteArray getPasswordSalt(quint64 userId) const override;

private:
    QSqlDatabase db() const;
    void initialize();
    void purge();
    void createTables();
};

} // namespace server
} // namespace whisper

#endif // SERVERSQLITEDATASTORAGE_H
