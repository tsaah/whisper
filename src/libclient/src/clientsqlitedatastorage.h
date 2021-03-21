#ifndef CLIENTSQLITEDATASTORAGE_H
#define CLIENTSQLITEDATASTORAGE_H

#include "iclientdatastorage.h"

namespace whisper {
namespace client {

class WHISPER_LIBCLIENT ClientSqliteDataStorage: public IClientDataStorage {
    Q_OBJECT
public:
    explicit ClientSqliteDataStorage(QObject* parent = nullptr);

    bool isStoredDeviceCertificateEmpty() const override;
    QByteArray restoreDeviceCertificate() const override;
    void storeDeviceCertificate(const QByteArray &deviceCertificate) override;

private:
    QSqlDatabase db() const;
    void initialize();
    void purge();
    void createTables();
};

} // namespace client
} // namespace whisper

#endif // CLIENTSQLITEDATASTORAGE_H
