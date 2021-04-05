#include "clientsqlitedatastorage.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

namespace whisper {
namespace client {

ClientSqliteDataStorage::ClientSqliteDataStorage(const QString& databaseFilename, QObject *parent)
    : IClientDataStorage(parent)
    , databaseFilename_(databaseFilename)
{
    initialize();
}

bool ClientSqliteDataStorage::isStoredDeviceCertificateEmpty() const {
    QSqlQuery q(db());
    bool ok = false;
    ok = q.exec("SELECT `id` FROM `deviceCertificate_table` WHERE `id` = 1 LIMIT 1;");
    SQLASSERT(ok, "query exec");
    return !q.next();
}

QByteArray ClientSqliteDataStorage::restoreDeviceCertificate() const {
    QSqlQuery q(db());
    bool ok = false;
    ok = q.exec("SELECT `certificate` FROM `deviceCertificate_table` WHERE `id` = 1 LIMIT 1;");
    SQLASSERT(ok, "query exec");
    ok = q.next();
    if (ok) {
        return q.value(0).toByteArray();
    } else {
        return {};
    }
}

void ClientSqliteDataStorage::storeDeviceCertificate(const QByteArray &deviceCertificate) {
    QSqlQuery q(db());
    bool ok = false;
    ok = q.prepare("INSERT OR REPLACE INTO `deviceCertificate_table` (`id`, `certificate`) VALUES (1, :cert);");
    SQLASSERT(ok, "query preparation");
    q.bindValue(":cert", deviceCertificate);
    ok = q.exec();
    SQLASSERT(ok, "query exec");
}

void ClientSqliteDataStorage::clearDeviceCertificate() {
    QSqlQuery q(db());
    bool ok = false;
    ok = q.prepare("DELETE FROM `deviceCertificate_table` WHERE `id` = 1;");
    SQLASSERT(ok, "query preparation");
    ok = q.exec();
    SQLASSERT(ok, "query exec");
}

bool ClientSqliteDataStorage::isStoredUserCertificateEmpty() const {
    QSqlQuery q(db());
    bool ok = false;
    ok = q.exec("SELECT `id` FROM `userCertificate_table` WHERE `id` = 1 LIMIT 1;");
    SQLASSERT(ok, "query exec");
    return !q.next();
}

QByteArray ClientSqliteDataStorage::restoreUserCertificate() const {
    QSqlQuery q(db());
    bool ok = false;
    ok = q.exec("SELECT `certificate` FROM `userCertificate_table` WHERE `id` = 1 LIMIT 1;");
    SQLASSERT(ok, "query exec");
    ok = q.next();
    if (ok) {
        return q.value(0).toByteArray();
    } else {
        return {};
    }
}

void ClientSqliteDataStorage::storeUserCertificate(const QByteArray &userCertificate) {
    QSqlQuery q(db());
    bool ok = false;
    ok = q.prepare("INSERT OR REPLACE INTO `userCertificate_table` (`id`, `certificate`) VALUES (1, :cert);");
    SQLASSERT(ok, "query preparation");
    q.bindValue(":cert", userCertificate);
    ok = q.exec();
    SQLASSERT(ok, "query exec");
}

void ClientSqliteDataStorage::clearUserCertificate() {
    QSqlQuery q(db());
    bool ok = false;
    ok = q.prepare("DELETE FROM `userCertificate_table` WHERE `id` = 1;");
    SQLASSERT(ok, "query preparation");
    ok = q.exec();
    SQLASSERT(ok, "query exec");
}

bool ClientSqliteDataStorage::isStoredUserIdEmpty() const {
    QSqlQuery q(db());
    bool ok = false;
    ok = q.exec("SELECT `id` FROM `userId_table` WHERE `id` = 1 LIMIT 1;");
    SQLASSERT(ok, "query exec");
    return !q.next();
}

quint64 ClientSqliteDataStorage::restoreUserId() const {
    QSqlQuery q(db());
    bool ok = false;
    ok = q.exec("SELECT `userId` FROM `userId_table` WHERE `id` = 1 LIMIT 1;");
    SQLASSERT(ok, "query exec");
    ok = q.next();
    if (ok) {
        return q.value(0).toULongLong();
    } else {
        return 0;
    }
}

void ClientSqliteDataStorage::storeUserId(quint64 userId) {
    QSqlQuery q(db());
    bool ok = false;
    ok = q.prepare("INSERT OR REPLACE INTO `userId_table` (`id`, `userId`) VALUES (1, :userId);");
    SQLASSERT(ok, "query preparation");
    q.bindValue(":userId", userId);
    ok = q.exec();
    SQLASSERT(ok, "query exec");
}

void ClientSqliteDataStorage::clearUserId() {
    QSqlQuery q(db());
    bool ok = false;
    ok = q.prepare("DELETE FROM `userId_table` WHERE `id` = 1;");
    SQLASSERT(ok, "query preparation");
    ok = q.exec();
    SQLASSERT(ok, "query exec");
}

QSqlDatabase ClientSqliteDataStorage::db() const {
    if (!QSqlDatabase::contains(databaseFilename_)) {
        auto db = QSqlDatabase::addDatabase("QSQLITE", databaseFilename_);
        db.setDatabaseName(databaseFilename_);
    }
    return QSqlDatabase::database(databaseFilename_);
}

void ClientSqliteDataStorage::initialize() {
    // check if database were initialized
    // true - check if table structure needs an upgrade
    // true - upgrade structure
    // false - initialize latest structure
    createTables();
}

void ClientSqliteDataStorage::purge() {
    // truncate database
}

void ClientSqliteDataStorage::createTables() {
    QSqlQuery q(db());
    bool ok = false;
    ok = q.exec(R"(
        CREATE TABLE IF NOT EXISTS `deviceCertificate_table` (
            `id` INTEGER PRIMARY KEY UNIQUE NOT NULL,
            `certificate` BLOB UNIQUE NOT NULL
        );
    )");
    SQLASSERT(ok, "query exec");

    ok = q.exec(R"(
        CREATE TABLE IF NOT EXISTS `userCertificate_table` (
            `id` INTEGER PRIMARY KEY UNIQUE NOT NULL,
            `certificate` BLOB UNIQUE NOT NULL
        );
    )");
    SQLASSERT(ok, "query exec");

    ok = q.exec(R"(
        CREATE TABLE IF NOT EXISTS `userId_table` (
            `id` INTEGER PRIMARY KEY UNIQUE NOT NULL,
            `userId` INT UNIQUE NOT NULL
        );
    )");
    SQLASSERT(ok, "query exec");
}


} // namespace client
} // namespace whisper
