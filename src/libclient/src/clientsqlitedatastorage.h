#ifndef CLIENTSQLITEDATASTORAGE_H
#define CLIENTSQLITEDATASTORAGE_H

#include "iclientdatastorage.h"

namespace whisper {
namespace client {

class WHISPER_LIBCLIENT ClientSqliteDataStorage: public IClientDataStorage {
    Q_OBJECT
public:
    explicit ClientSqliteDataStorage(const QString& databaseFilename, QObject* parent = nullptr);

    bool isStoredDeviceCertificateEmpty() const override;
    QByteArray restoreDeviceCertificate() const override;
    void storeDeviceCertificate(const QByteArray &deviceCertificate) override;
    void clearDeviceCertificate() override;

    bool isStoredUserCertificateEmpty() const override;
    QByteArray restoreUserCertificate() const override;
    void storeUserCertificate(const QByteArray &userCertificate) override;
    void clearUserCertificate() override;

    bool isStoredUserIdEmpty() const override;
    quint64 restoreUserId() const override;
    void storeUserId(quint64 userId) override;
    void clearUserId() override;

private:
    QSqlDatabase db() const;
    void initialize();
    void purge();
    void createTables();

    QString databaseFilename_;
};

} // namespace client
} // namespace whisper

#endif // CLIENTSQLITEDATASTORAGE_H
