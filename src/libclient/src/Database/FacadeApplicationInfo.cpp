#pragma once

#include "FacadeApplicationInfo.h"
#include <Crypto/Crypto.h>

#include <sqlassert.h>

#include <QSqlQuery>
#include <QSqlError>

namespace whisper::client::database {

const Version FacadeApplicationInfo::VERSION = 1;
const QString FacadeApplicationInfo::TABLE_NAME = "applicationInfo_table";
const int FacadeApplicationInfo::STATIC_ID = 1;

FacadeApplicationInfo::FacadeApplicationInfo(QSqlDatabase& db, const QByteArray& password)
    : IFacadeApplicationInfo()
    , db_(db)
{
    Q_ASSERT(db_.isValid());
    Q_ASSERT(db_.open());
    createTable();
    load(password);
}

void FacadeApplicationInfo::clearTable() {
    QSqlQuery q(db_);

    SQL_ASSERT(q.prepare(R"(
        UPDATE `:tableName` SET
            `id` = :staticId,
            `version` = :version,
            `encryptedDeviceKey` = NULL,
            `encryptedUserKey` = NULL,
            `userId` = NULL,
            `passwordSalt = NULL,
            `passwordHash = NULL;
    )"), q);
    q.bindValue(":tableName", TABLE_NAME);

    SQL_ASSERT(q.exec(), q);

    deviceKey_ = {};
    userKey_ = {};
    userId_ = 0;
    passwordSalt_.clear();
    passwordHash_.clear();
}

bool FacadeApplicationInfo::checkPassword(const QByteArray &password) const {
    if (password.isEmpty()) {
        return false;
    }

    QByteArray saltedPassword;
    saltedPassword.append(passwordSalt_);
    saltedPassword.append(password);
    const auto passwordHash = common::crypto::Crypto::secureHash(saltedPassword);
    saltedPassword.fill('\0');
    return passwordHash == passwordHash_;
}

void FacadeApplicationInfo::resetPasswordAndClear(const QByteArray &password) {
    Q_ASSERT(!password.isEmpty());
    clearTable();

    QSqlQuery q(db_);

    SQL_ASSERT(q.prepare(R"(
        UPDATE `:tableName` (
            `passwordSalt`, `passwordHash`
        ) VALUES (
            :passwordSalt, :passwordHash
        ) WHERE `id` = :staticId;
    )"), q);
    q.bindValue(":tableName", TABLE_NAME);
    q.bindValue(":staticId", STATIC_ID);

    const auto passwordSalt = common::crypto::Crypto::generateSalt();
    q.bindValue(":passwordSalt", passwordSalt);
    QByteArray saltedPassword;
    saltedPassword.append(passwordSalt);
    saltedPassword.append(password);
    const auto passwordHash = common::crypto::Crypto::secureHash(saltedPassword);
    saltedPassword.fill('\0');
    q.bindValue(":passwordHash", passwordHash);

    SQL_ASSERT(q.exec(), q);
}

bool FacadeApplicationInfo::changePassword(const QByteArray &oldPassword, const QByteArray &newPassword) {
    if (oldPassword.isEmpty() || newPassword.isEmpty() || !checkPassword(oldPassword)) {
        return false;
    }

    QSqlQuery q(db_);

    SQL_ASSERT(q.prepare(R"(
        UPDATE `:tableName` (
            `passwordSalt`, `passwordHash`
        ) VALUES (
            :passwordSalt, :passwordHash
        ) WHERE `id` = :staticId;
    )"), q);
    q.bindValue(":tableName", TABLE_NAME);
    q.bindValue(":staticId", STATIC_ID);

    const auto newPasswordSalt = common::crypto::Crypto::generateSalt();
    q.bindValue(":passwordSalt", newPasswordSalt);

    QByteArray saltedPassword;
    saltedPassword.append(newPasswordSalt);
    saltedPassword.append(newPassword);
    const auto newPasswordHash = common::crypto::Crypto::secureHash(saltedPassword);
    saltedPassword.fill('\0');
    q.bindValue(":passwordHash", newPasswordHash);

    if (deviceKey_.isValid()) {
        const auto encryptedDeviceKey = common::crypto::Crypto::encryptAES(deviceKey_.toByteArray(), newPassword);
        q.bindValue(":encryptedDeviceKey", encryptedDeviceKey);
    } else {
        q.bindValue(":encryptedDeviceKey", QVariant(QVariant::ByteArray));
    }

    if (userKey_.isValid()) {
        const auto encryptedUserKey = common::crypto::Crypto::encryptAES(userKey_.toByteArray(), newPassword);
        q.bindValue(":encryptedUserKey", encryptedUserKey);
    } else {
        q.bindValue(":encryptedUserKey", QVariant(QVariant::ByteArray));
    }

    SQL_ASSERT(q.exec(), q);

    return true;
}

void FacadeApplicationInfo::setDeviceKey(const common::crypto::PrivateKey &key, const QByteArray &password) {
    if (!checkPassword(password) || !key.isValid()) {
        return;
    }

    QSqlQuery q(db_);

    SQL_ASSERT(q.prepare(R"(
        UPDATE `:tableName` (
            `encryptedDeviceKey`
        ) VALUES (
            :encryptedDeviceKey
        ) WHERE `id` = :staticId;
    )"), q);
    q.bindValue(":tableName", TABLE_NAME);
    q.bindValue(":staticId", STATIC_ID);

    const auto encryptedDeviceKey = common::crypto::Crypto::encryptAES(key.toByteArray(), password);
    q.bindValue(":encryptedDeviceKey", encryptedDeviceKey);

    SQL_ASSERT(q.exec(), q);

    deviceKey_ = key;
}

void FacadeApplicationInfo::setUserKey(const common::crypto::PrivateKey &key, const QByteArray& password) {
    if (!checkPassword(password) || !key.isValid()) {
        return;
    }

    QSqlQuery q(db_);

    SQL_ASSERT(q.prepare(R"(
        UPDATE `:tableName` (
            `encryptedUserKey`
        ) VALUES (
            :encryptedUserKey
        ) WHERE `id` = :staticId;
    )"), q);
    q.bindValue(":tableName", TABLE_NAME);
    q.bindValue(":staticId", STATIC_ID);

    const auto encryptedUserKey = common::crypto::Crypto::encryptAES(key.toByteArray(), password);
    q.bindValue(":encryptedUserKey", encryptedUserKey);

    SQL_ASSERT(q.exec(), q);

    userKey_ = key;
}

void FacadeApplicationInfo::setUserId(UserId id) {
    if (id == 0) {
        return;
    }

    QSqlQuery q(db_);

    SQL_ASSERT(q.prepare(R"(
        UPDATE `:tableName` (
            `userId`
        ) VALUES (
            :userId
        ) WHERE `id` = :staticId;
    )"), q);
    q.bindValue(":tableName", TABLE_NAME);
    q.bindValue(":staticId", STATIC_ID);

    q.bindValue(":userId", id);

    SQL_ASSERT(q.exec(), q);
}


void FacadeApplicationInfo::createTable() {
    if (hasTable()) {
       return;
    }

    {
        QSqlQuery q(db_);

        SQL_ASSERT(q.prepare(R"(
            CREATE TABLE `:tableName` (
                `id` INTEGER PRIMARY KEY UNIQUE NOT NULL,
                `version` INTEGER UNIQUE NOT NULL DEFAULT :version,
                `encryptedDeviceKey` BLOB UNIQUE DEFAULT NULL,
                `encryptedUserKey` BLOB UNIQUE DEFAULT NULL,
                `userId` INTEGER UNIQUE DEFAULT NULL,
                `passwordSalt` TEXT UNIQUE DEFAULT NULL,
                `passwordHash` TEXT UNIQUE DEFAULT NULL
            );
        )"), q);
        q.bindValue(":tableName", TABLE_NAME);
        q.bindValue(":version", VERSION);

        SQL_ASSERT(q.exec(), q);
    }
    {
        QSqlQuery q(db_);

        SQL_ASSERT(q.prepare(R"(
            INSERT INTO `:tableName` (
                `id`, `version`, `encryptedDeviceKey`, `encryptedUserKey`, `userId`, `passwordSalt`, `passwordHash`
            ) VALUES (
                :staticId, :version, NULL, NULL, NULL, NULL, NULL
            );
        )"), q);
        q.bindValue(":tableName", TABLE_NAME);
        q.bindValue(":version", VERSION);
        q.bindValue(":staticId", STATIC_ID);

        SQL_ASSERT(q.exec(), q);
    }
    version_ = VERSION;
}

bool FacadeApplicationInfo::hasTable() const {
    QSqlQuery q(db_);

    SQL_ASSERT(q.prepare(R"(
        SELECT `name` FROM `sqlite_master` WHERE `type` = 'table' AND `name` = ':tableName';
    )"), q);
    q.bindValue(":tableName", TABLE_NAME);

    SQL_ASSERT(q.exec(), q);

    const auto querySize = q.size();
    Q_ASSERT(querySize == 1 || querySize == 0);

    return querySize == 1;
}

void FacadeApplicationInfo::load(const QByteArray &password) const {
    if (!checkPassword(password)) {
        return;
    }

    QSqlQuery q(db_);

    SQL_ASSERT(q.prepare(R"(
        SELECT
            `version`,
            `encryptedDeviceKey`,
            `encryptedUserKey`,
            `userId`,
            `passwordSalt`,
            `passwordHash`
        FROM `:tableName` WHERE `id` = :staticId;
    )"), q);
    q.bindValue(":tableName", TABLE_NAME);
    q.bindValue(":staticId", STATIC_ID);

    Q_ASSERT(q.size() == 1);

    Q_ASSERT(q.next());

    const auto version = q.value("version").toUInt();
    // if version is older than expected - updated record

    const auto encryptedDeviceKey = q.value("encryptedDeviceKey").toByteArray();
    const auto encryptedUserKey = q.value("encryptedUserKey").toByteArray();
    userId_ = q.value("userId").toUInt();
    passwordSalt_ = q.value("passwordKey").toByteArray();
    passwordHash_ = q.value("passwordHash").toByteArray();

    if (encryptedDeviceKey.isEmpty()) {
        deviceKey_ = {};
    } else {
        const auto decryptedKey = common::crypto::Crypto::decryptAES(encryptedDeviceKey, password);
        deviceKey_.fromByteArray(decryptedKey);
    }

    if (encryptedUserKey.isEmpty()) {
        userKey_ = {};
    } else {
        const auto decryptedKey = common::crypto::Crypto::decryptAES(encryptedUserKey, password);
        userKey_.fromByteArray(decryptedKey);
    }
}

} // namespace whisper::client::database
