#include "ClientSqliteDataStorage.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QTimeZone>

namespace whisper::client {

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

void ClientSqliteDataStorage::cleanContactList() {
    QSqlQuery q(db());
    SQLASSERT(q.exec(R"(
        DELETE FROM `contacts_table`;
    )"), "query exec");
}

QList<Contact> ClientSqliteDataStorage::getContactList() const {
    QSqlQuery q(db());
    SQLASSERT(q.exec("SELECT `userId`, `certificate`, `approved`, `updateTimestamp` FROM `contacts_table` ORDER BY `updateTimestamp` DESC;"), "query exec");
    QList<Contact> list;
    while (q.next()) {
        Contact c;
        c.userId = q.value("userId").toULongLong();
        c.certificate = q.value("certificate").toByteArray();
        c.approved = q.value("approved").toBool();
        c.updateTimestamp = QDateTime::fromMSecsSinceEpoch(q.value("updateTimestamp").toLongLong(), QTimeZone::systemTimeZone());
        list.append(c);
    }
    return list;
}

void ClientSqliteDataStorage::storeContactList(const QList<Contact> &contactList) {
    SQLDBASSERT(db().transaction(),"begin transaction");
    QSqlQuery q(db());
    SQLASSERT(q.exec("(DELETE FROM `contacts_table`;"), "query exec");
    for (const auto& c: contactList) {
        SQLASSERT(q.prepare("INSERT OR REPLACE INTO `contacts_table` (`userId`, `certificate`, `approved`, `updateTimestamp`) VALUES (:userId, :certificate, :approved, :updateTimestamp);"), "query preparation");
        q.bindValue(":userId", c.userId);
        q.bindValue(":certificate", c.certificate);
        q.bindValue(":approved", c.approved);
        q.bindValue(":updateTimestamp", c.updateTimestamp.toUTC().toMSecsSinceEpoch());
        SQLASSERT(q.exec(), "query exec");
    }
    SQLDBASSERT(db().commit(), "commit transaction");
}

void ClientSqliteDataStorage::approveContact(quint64 userId, const QByteArray &certificate) {
    QSqlQuery q(db());
    SQLASSERT(q.exec("UPDATE `contacts_table` SET `certificate` = :certificate, `approved` = :approved, `updateTimestamp` = :updateTimestamp WHERE `userId` = :userId;"), "query exec");
    q.bindValue(":userId", userId);
    q.bindValue(":certificate", certificate);
    q.bindValue(":approved", true);
    q.bindValue(":updateTimestamp", QDateTime::currentDateTimeUtc().toMSecsSinceEpoch());
    SQLASSERT(q.exec(), "query exec");
}

void ClientSqliteDataStorage::addUnapprovedContact(quint64 userId) {
    QSqlQuery q(db());
    SQLASSERT(q.prepare("INSERT OR REPLACE INTO `contacts_table` (`userId`, `approved`, `updateTimestamp`) VALUES (:userId, :approved, :updateTimestamp);"), "query preparation");
    q.bindValue(":userId", userId);
    q.bindValue(":approved", false);
    q.bindValue(":updateTimestamp", QDateTime::currentDateTimeUtc().toMSecsSinceEpoch());
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
        CREATE TABLE IF NOT EXISTS `contacts_table` (
            `id` INTEGER PRIMARY KEY UNIQUE NOT NULL,
            `userId` INT UNIQUE NOT NULL,
            `certificate` BLOB,
            `approved` BOOL,
            `updateTimestamp` INT
        );
    )"), "query exec");
}

} // namespace whisper::client
