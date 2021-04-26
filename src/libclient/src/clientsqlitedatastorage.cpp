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
    SQLASSERT(q.exec("SELECT `id` FROM `deviceCertificate_table` WHERE `id` = 1 LIMIT 1;"), "query exec");
    return !q.next();
}

QByteArray ClientSqliteDataStorage::restoreDeviceCertificate() const {
    QSqlQuery q(db());
    QByteArray result;
    SQLASSERT(q.exec("SELECT `certificate` FROM `deviceCertificate_table` WHERE `id` = 1 LIMIT 1;"), "query exec");
    if (q.next()) {
        result = q.value(0).toByteArray();
    }
    return result;
}

void ClientSqliteDataStorage::storeDeviceCertificate(const QByteArray &deviceCertificate) {
    QSqlQuery q(db());
    SQLASSERT(q.prepare("INSERT OR REPLACE INTO `deviceCertificate_table` (`id`, `certificate`) VALUES (1, :cert);"), "query preparation");
    q.bindValue(":cert", deviceCertificate);
    SQLASSERT(q.exec(), "query exec");
}

void ClientSqliteDataStorage::clearDeviceCertificate() {
    QSqlQuery q(db());
    SQLASSERT(q.prepare("DELETE FROM `deviceCertificate_table` WHERE `id` = 1;"), "query preparation");
    SQLASSERT(q.exec(), "query exec");
}

bool ClientSqliteDataStorage::isStoredUserCertificateEmpty() const {
    QSqlQuery q(db());
    SQLASSERT(q.exec("SELECT `id` FROM `userCertificate_table` WHERE `id` = 1 LIMIT 1;"), "query exec");
    return !q.next();
}

QByteArray ClientSqliteDataStorage::restoreUserCertificate() const {
    QSqlQuery q(db());
    QByteArray result;
    SQLASSERT(q.exec("SELECT `certificate` FROM `userCertificate_table` WHERE `id` = 1 LIMIT 1;"), "query exec");
    if (q.next()) {
        result = q.value(0).toByteArray();
    }
    return result;
}

void ClientSqliteDataStorage::storeUserCertificate(const QByteArray &userCertificate) {
    QSqlQuery q(db());
    SQLASSERT(q.prepare("INSERT OR REPLACE INTO `userCertificate_table` (`id`, `certificate`) VALUES (1, :cert);"), "query preparation");
    q.bindValue(":cert", userCertificate);
    SQLASSERT(q.exec(), "query exec");
}

void ClientSqliteDataStorage::clearUserCertificate() {
    QSqlQuery q(db());
    SQLASSERT(q.prepare("DELETE FROM `userCertificate_table` WHERE `id` = 1;"), "query preparation");
    SQLASSERT(q.exec(), "query exec");
}

bool ClientSqliteDataStorage::isStoredUserIdEmpty() const {
    QSqlQuery q(db());
    SQLASSERT(q.exec("SELECT `id` FROM `userId_table` WHERE `id` = 1 LIMIT 1;"), "query exec");
    return !q.next();
}

quint64 ClientSqliteDataStorage::restoreUserId() const {
    QSqlQuery q(db());
    quint64 result = 0;
    SQLASSERT(q.exec("SELECT `userId` FROM `userId_table` WHERE `id` = 1 LIMIT 1;"), "query exec");
    if (q.next()) {
        result = q.value(0).toULongLong();
    }
    return result;
}

void ClientSqliteDataStorage::storeUserId(quint64 userId) {
    QSqlQuery q(db());
    SQLASSERT(q.prepare("INSERT OR REPLACE INTO `userId_table` (`id`, `userId`) VALUES (1, :userId);"), "query preparation");
    q.bindValue(":userId", userId);
    SQLASSERT(q.exec(), "query exec");
}

void ClientSqliteDataStorage::clearUserId() {
    QSqlQuery q(db());
    SQLASSERT(q.prepare("DELETE FROM `userId_table` WHERE `id` = 1;"), "query preparation");
    SQLASSERT(q.exec(), "query exec");
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
    SQLASSERT(q.exec(R"(
        CREATE TABLE IF NOT EXISTS `deviceCertificate_table` (
            `id` INTEGER PRIMARY KEY UNIQUE NOT NULL,
            `certificate` BLOB UNIQUE NOT NULL
        );
    )"), "query exec");

    SQLASSERT(q.exec(R"(
        CREATE TABLE IF NOT EXISTS `userCertificate_table` (
            `id` INTEGER PRIMARY KEY UNIQUE NOT NULL,
            `certificate` BLOB UNIQUE NOT NULL
        );
    )"), "query exec");

    SQLASSERT(q.exec(R"(
        CREATE TABLE IF NOT EXISTS `userId_table` (
            `id` INTEGER PRIMARY KEY UNIQUE NOT NULL,
            `userId` INT UNIQUE NOT NULL
        );
    )"), "query exec");
}


} // namespace client
} // namespace whisper
