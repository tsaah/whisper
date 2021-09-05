#include "ServerSqliteDataStorage.h"

#include <types.h>
#include <log.h>

#include <QHash>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

namespace whisper::server {

ServerSqliteDataStorage::ServerSqliteDataStorage(QObject *parent): IServerDataStorage(parent) {
    initialize();
}

bool ServerSqliteDataStorage::deviceExists(const QByteArray &deviceCert) const {
    const auto hash = qHash(deviceCert);
    QSqlQuery q(db());
    bool result = false;
    SQLASSERT(q.prepare("SELECT `hash` FROM `device_table` WHERE `hash` = :hash LIMIT 1;"), "query preparation");
    q.bindValue(":hash", hash);
    SQLASSERT(q.exec(), "query exec");
    if (q.next()) {
        const auto dbHash = q.value("hash").toUInt();
        wDebug << dbHash << hash << deviceCert.left(10);
        result = dbHash == hash;
    }
    return result;
}

void ServerSqliteDataStorage::rememberDevice(const QByteArray &deviceCert) {
    QSqlQuery q(db());
    SQLASSERT(q.prepare("INSERT INTO `device_table` (`hash`, `certificate`) VALUES (:hash, :cert);"), "query preparation");
    q.bindValue(":hash", qHash(deviceCert));
    q.bindValue(":cert", deviceCert);
    SQLASSERT(q.exec(), "query exec");
}

bool ServerSqliteDataStorage::userExists(const QByteArray &userCert) const {
    QSqlQuery q(db());
    SQLASSERT(q.prepare("SELECT `id` FROM `user_table` WHERE `hash` = :hash LIMIT 1;"), "query preparation");
    q.bindValue(":hash", qHash(userCert));
    SQLASSERT(q.exec(), "query exec");
    return q.next();
}

bool ServerSqliteDataStorage::userIdExists(quint64 userId) const {
    QSqlQuery q(db());
    bool result = false;
    SQLASSERT (q.prepare("SELECT `id` FROM `user_table` WHERE `id` = :id LIMIT 1;"), "query preparation");
    q.bindValue(":id", userId);
    SQLASSERT(q.exec(), "query exec");
    if (q.next()) {
        const auto dbUserId = q.value("id").toUInt();
        result = dbUserId == userId;
    }
    return result;
}

void ServerSqliteDataStorage::rememberNewUser(quint64 userId, const QByteArray &userCert, const QByteArray &deviceCert, const QByteArray &passwordHash, const QByteArray& passwordSalt) {
    QSqlQuery q(db());
    SQLASSERT(q.prepare("INSERT INTO `user_table` (`hash`, `id`, `certificate`, `passwordHash`, `passwordSalt`) VALUES (:hash, :id, :cert, :passwordHash, :passwordSalt);"), "query preparation");
    q.bindValue(":hash", qHash(userCert));
    q.bindValue(":id", userId);
    q.bindValue(":cert", userCert);
    q.bindValue(":passwordHash", passwordHash);
    q.bindValue(":passwordSalt", passwordSalt);
    SQLASSERT(q.exec(), "query exec");
    q.finish();
    SQLASSERT(q.prepare("INSERT INTO `user_devices_table` (`userHash`, `deviceHash`) VALUES (:userHash, :deviceHash);"), "query preparation");
    q.bindValue(":userHash", qHash(userCert));
    q.bindValue(":deviceHash", qHash(deviceCert));
    SQLASSERT(q.exec(), "query exec");
}

QByteArray ServerSqliteDataStorage::getUserCertificate(quint64 userId) const {
    QSqlQuery q(db());
    QByteArray result;
    SQLASSERT(q.prepare("SELECT `certificate` FROM `user_table` WHERE `id` = :id;"), "query preparation");
    q.bindValue(":id", userId);
    SQLASSERT(q.exec(), "query exec");
    if (q.next()) {
        result = q.value("certificate").toByteArray();
    }
    return result;
}

QByteArray ServerSqliteDataStorage::getPasswordHash(quint64 userId) const {
    QSqlQuery q(db());
    QByteArray result;
    SQLASSERT(q.prepare("SELECT `passwordHash` FROM `user_table` WHERE `id` = :id;"), "query preparation");
    q.bindValue(":id", userId);
    SQLASSERT(q.exec(), "query exec");
    if (q.next()) {
        result = q.value("passwordHash").toByteArray();
    }
    return result;
}

QByteArray ServerSqliteDataStorage::getPasswordSalt(quint64 userId) const {
    QSqlQuery q(db());
    QByteArray result;
    SQLASSERT(q.prepare("SELECT `passwordSalt` FROM `user_table` WHERE `id` = :id;"), "query preparation");
    q.bindValue(":id", userId);
    SQLASSERT(q.exec(), "query exec");
    if (q.next()) {
        result = q.value("passwordSalt").toByteArray();
    }
    return result;
}

QSqlDatabase ServerSqliteDataStorage::db() const {
    const QString dbName = "server_data_storage.sqlite";
    if (!QSqlDatabase::contains(dbName)) {
        auto db = QSqlDatabase::addDatabase("QSQLITE", dbName);
        db.setDatabaseName(dbName);
    }
    return QSqlDatabase::database(dbName);
}

void ServerSqliteDataStorage::initialize() {
    // check if database were initialized
    // true - check if table structure needs an upgrade
    // true - upgrade structure
    // false - initialize latest structure
    createTables();
}

void ServerSqliteDataStorage::purge() {
    // truncate database
}

void ServerSqliteDataStorage::createTables() {
    QSqlQuery q(db());

    SQLASSERT(q.exec(R"(
        CREATE TABLE IF NOT EXISTS `device_table` (
            `hash` INTEGER PRIMARY KEY UNIQUE NOT NULL,
            `certificate` BLOB UNIQUE NOT NULL
        );
    )"), "query exec");

    SQLASSERT(q.exec(R"(
        CREATE TABLE IF NOT EXISTS `user_table` (
            `hash` INTEGER PRIMARY KEY UNIQUE NOT NULL,
            `id` INTEGER UNIQUE NOT NULL,
            `certificate` BLOB UNIQUE NOT NULL,
            `passwordHash` BLOB NOT NULL,
            `passwordSalt` BLOB NOT NULL
        );
    )"), "query exec");

    SQLASSERT(q.exec(R"(
        CREATE TABLE IF NOT EXISTS `user_devices_table` (
            `userHash` INT NOT NULL,
            `deviceHash` INT NOT NULL
        );
    )"), "query exec");
}

} // namespace whisper::server


