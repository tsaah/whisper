#include "serversqlitedatastorage.h"

#include <types.h>

#include <QHash>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

namespace whisper {
namespace server {

ServerSqliteDataStorage::ServerSqliteDataStorage(QObject *parent): IServerDataStorage(parent) {
    initialize();
}

bool ServerSqliteDataStorage::deviceExists(const QByteArray &deviceCert) const {
    const auto hash = qHash(deviceCert);
    QSqlQuery q(db());
    bool ok = false;
    ok = q.prepare("SELECT EXISTS(SELECT 1 FROM `device_table` WHERE `hash` = :hash LIMIT 1);");
    SQLASSERT(ok, "query preparation");
    q.bindValue(":hash", hash);
    ok = q.exec();
    SQLASSERT(ok, "query exec");
    ok = q.next();
    SQLASSERT(ok, "query next");
    return q.value(0).toInt() == 1;
}

void ServerSqliteDataStorage::rememberDevice(const QByteArray &deviceCert) {
    QSqlQuery q(db());
    bool ok = false;
    ok = q.prepare("INSERT INTO `device_table` (`hash`, `certificate`) VALUES (:hash, :cert);");
    SQLASSERT(ok, "query preparation");
    q.bindValue(":hash", qHash(deviceCert));
    q.bindValue(":cert", deviceCert);
    ok = q.exec();
    SQLASSERT(ok, "query exec");
}

bool ServerSqliteDataStorage::userExists(const QByteArray &userCert) const {
    QSqlQuery q(db());
    bool ok = false;
    ok = q.prepare("SELECT EXISTS(SELECT 1 FROM `user_table` WHERE `hash` = :hash LIMIT 1);");
    SQLASSERT(ok, "query preparation");
    q.bindValue(":hash", qHash(userCert));
    ok = q.exec();
    SQLASSERT(ok, "query exec");
    ok = q.next();
    SQLASSERT(ok, "query next");
    return q.value(0).toInt() == 1;
}

bool ServerSqliteDataStorage::userIdExists(quint64 userId) const {
    QSqlQuery q(db());
    bool ok = false;
    ok = q.prepare("SELECT EXISTS(SELECT 1 FROM `user_table` WHERE `id` = :id LIMIT 1);");
    SQLASSERT(ok, "query preparation");
    q.bindValue(":id", userId);
    ok = q.exec();
    SQLASSERT(ok, "query exec");
    ok = q.next();
    SQLASSERT(ok, "query next");
    return q.value(0).toInt() == 1;
}

void ServerSqliteDataStorage::rememberNewUser(quint64 userId, const QByteArray &userCert, const QByteArray &deviceCert, const QByteArray &passwordHash, const QByteArray& passwordSalt) {
    QSqlQuery q(db());
    bool ok = false;
    ok = q.prepare("INSERT INTO `user_table` (`hash`, `id`, `certificate`, `passwordHash`, `passwordSalt`) VALUES (:hash, :id, :cert, :passwordHash, :passwordSalt);");
    SQLASSERT(ok, "query preparation");
    q.bindValue(":hash", qHash(userCert));
    q.bindValue(":id", userId);
    q.bindValue(":cert", userCert);
    q.bindValue(":passwordHash", passwordHash);
    q.bindValue(":passwordSalt", passwordSalt);
    ok = q.exec();
    SQLASSERT(ok, "query exec");
    q.finish();

    ok = q.prepare("INSERT INTO `user_devices_table` (`userHash`, `deviceHash`) VALUES (:userHash, :deviceHash);");
    SQLASSERT(ok, "query preparation");
    q.bindValue(":userHash", qHash(userCert));
    q.bindValue(":deviceHash", qHash(deviceCert));
    ok = q.exec();
    SQLASSERT(ok, "query exec");

}

QByteArray ServerSqliteDataStorage::getUserCertificate(quint64 userId) const {
    QSqlQuery q(db());
    bool ok = false;
    ok = q.prepare("SELECT `certificate` FROM `user_table` WHERE `id` = :id;");
    SQLASSERT(ok, "query preparation");
    q.bindValue(":id", userId);
    ok = q.exec();
    SQLASSERT(ok, "query exec");
    ok = q.next();
    if (ok) {
        return q.value("certificate").toByteArray();
    } else {
        return {};
    }
}

QByteArray ServerSqliteDataStorage::getPasswordHash(quint64 userId) const {
    QSqlQuery q(db());
    bool ok = false;
    ok = q.prepare("SELECT `passwordHash` FROM `user_table` WHERE `id` = :id;");
    SQLASSERT(ok, "query preparation");
    q.bindValue(":id", userId);
    ok = q.exec();
    SQLASSERT(ok, "query exec");
    ok = q.next();
    if (ok) {
        return q.value("passwordHash").toByteArray();
    } else {
        return {};
    }
}

QByteArray ServerSqliteDataStorage::getPasswordSalt(quint64 userId) const {
    QSqlQuery q(db());
    bool ok = false;
    ok = q.prepare("SELECT `passwordSalt` FROM `user_table` WHERE `id` = :id;");
    SQLASSERT(ok, "query preparation");
    q.bindValue(":id", userId);
    ok = q.exec();
    SQLASSERT(ok, "query exec");
    ok = q.next();
    if (ok) {
        return q.value("passwordSalt").toByteArray();
    } else {
        return {};
    }
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
    bool ok = q.exec(R"(
            CREATE TABLE IF NOT EXISTS `device_table` (
                `hash` INTEGER PRIMARY KEY UNIQUE NOT NULL,
                `certificate` BLOB UNIQUE NOT NULL
            );
        )");
    SQLASSERT(ok, "query exec");
    ok = q.exec(R"(
            CREATE TABLE IF NOT EXISTS `user_table` (
                `hash` INTEGER PRIMARY KEY UNIQUE NOT NULL,
                `id` INTEGER UNIQUE NOT NULL,
                `certificate` BLOB UNIQUE NOT NULL,
                `passwordHash` BLOB NOT NULL,
                `passwordSalt` BLOB NOT NULL
            );
        )");
    SQLASSERT(ok, "query exec");
    ok = q.exec(R"(
            CREATE TABLE IF NOT EXISTS `user_devices_table` (
                `userHash` INT NOT NULL,
                `deviceHash` INT NOT NULL
            );
        )");
    SQLASSERT(ok, "query exec");
}

} // namespace server
} // namespace whisper


