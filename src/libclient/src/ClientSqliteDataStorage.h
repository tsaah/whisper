#pragma once

#include "IClientDataStorage.h"

namespace whisper::client {

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

    void cleanContactList() override;
    QList<Contact> getContactList() const override;
    void storeContactList(const QList<Contact>& contactList) override;
    void approveContact(quint64 userId, const QByteArray& certificate) override;
    void addUnapprovedContact(quint64 userId) override;

private:
    QSqlDatabase db() const;
    void initialize();
    void purge();
    void createTables();

    QString databaseFilename_;
};

} // namespace whisper::client
