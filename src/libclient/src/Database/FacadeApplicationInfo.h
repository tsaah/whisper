#pragma once

#include "IFacadeApplicationInfo.h"

#include <QSqlDatabase>

namespace whisper::client::database {

class WHISPER_LIBCLIENT FacadeApplicationInfo final: public IFacadeApplicationInfo {
public:
    FacadeApplicationInfo(QSqlDatabase& db, const QByteArray &password);

    void clearTable() override;

    bool checkPassword(const QByteArray &password) const override;
    void resetPasswordAndClear(const QByteArray &password) override;
    bool changePassword(const QByteArray& oldPassword, const QByteArray& newPassword) override;

    void setDeviceKey(const common::crypto::PrivateKey &key, const QByteArray &password) override;
    void setUserKey(const common::crypto::PrivateKey &key, const QByteArray& password) override;
    void setUserId(UserId id) override;

protected:
    void createTable() override;
    bool hasTable() const override;
    void load(const QByteArray& password) const override;

private:
    QSqlDatabase db_;
    static const Version VERSION;
    static const QString TABLE_NAME;
    static const int STATIC_ID;

    mutable Version version_{ 0 };
    mutable common::crypto::PrivateKey deviceKey_;
    mutable common::crypto::PrivateKey userKey_;
    mutable UserId userId_{ 0 };
    mutable QByteArray passwordSalt_;
    mutable QByteArray passwordHash_;
};

} // namespace whisper::client::database
