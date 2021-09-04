#pragma once

#include "libclient_export.h"

#include <Crypto/PrivateKey.h>

namespace whisper::client {

using Version = quint64;
using UserId = quint64;

class WHISPER_LIBCLIENT IFacadeApplicationInfo {
public:
    virtual ~IFacadeApplicationInfo() = default;

    virtual void clearTable() = 0;

    virtual bool checkPassword(const QByteArray& password) const = 0;
    virtual void resetPasswordAndClear(const QByteArray& password) = 0;
    virtual bool changePassword(const QByteArray& oldPassword, const QByteArray& newPassword) = 0;

    virtual void setDeviceKey(const common::crypto::PrivateKey& key, const QByteArray& password) = 0;
    virtual void setUserKey(const common::crypto::PrivateKey& key, const QByteArray& password) = 0;
    virtual void setUserId(UserId id) = 0;

    virtual common::crypto::PrivateKey deviceKey(const QByteArray& password) const = 0;
    virtual common::crypto::PrivateKey userKey(const QByteArray& password) const = 0;
    virtual UserId userId() const = 0;

protected:
    virtual void createTable() = 0;
    virtual bool hasTable() const = 0;
    virtual void load() const = 0;
};

} // namespace whisper::client
