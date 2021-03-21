#include "clientsqlitedatastorage.h"

#include <QSqlQuery>
#include <QVariant>

namespace whisper {
namespace client {

ClientSqliteDataStorage::ClientSqliteDataStorage(QObject *parent)
    : IClientDataStorage(parent)
{
    initialize();
}

bool ClientSqliteDataStorage::isStoredDeviceCertificateEmpty() const {
    QSqlQuery q(db());
    bool ok = false;
    ok = q.exec("SELECT EXISTS(SELECT 1 FROM `deviceCertificate_table` WHERE `id` = 1 LIMIT 1);");
    Q_ASSERT_X(ok, __FUNCTION__, "query exec");
    ok = q.next();
    Q_ASSERT_X(ok, __FUNCTION__, "query next");
    return q.value(0).toInt() != 1;
}

QByteArray ClientSqliteDataStorage::restoreDeviceCertificate() const {
    QSqlQuery q(db());
    bool ok = false;
    ok = q.exec("SELECT `certificate` FROM `deviceCertificate_table` WHERE `id` = 1 LIMIT 1;");
    Q_ASSERT_X(ok, __FUNCTION__, "query exec");
    ok = q.next();
    Q_ASSERT_X(ok, __FUNCTION__, "query next");
    return QByteArray::fromBase64(q.value(0).toByteArray());
}

void ClientSqliteDataStorage::storeDeviceCertificate(const QByteArray &deviceCertificate) {
    QSqlQuery q(db());
    bool ok = false;
    ok = q.prepare("INSERT OR REPLACE INTO `deviceCertificate_table` (`id`, `certificate`) VALUES (1, :cert);");
    Q_ASSERT_X(ok, __FUNCTION__, "query preparation");
    q.bindValue(":cert", deviceCertificate.toBase64());
    ok = q.exec();
    Q_ASSERT_X(ok, __FUNCTION__, "query exec");
}

QSqlDatabase ClientSqliteDataStorage::db() const {
    const QString dbName = "client_data_storage.sqlite";
    if (!QSqlDatabase::contains(dbName)) {
        auto db = QSqlDatabase::addDatabase("QSQLITE", dbName);
        db.setDatabaseName(dbName);
    }
    return QSqlDatabase::database(dbName);
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
    const bool ok = q.exec(R"(
            CREATE TABLE IF NOT EXISTS `deviceCertificate_table` (
                `id` INTEGER PRIMARY KEY UNIQUE NOT NULL,
                `certificate` BLOB UNIQUE NOT NULL
            );
        )");
    Q_ASSERT_X(ok, __FUNCTION__, "query exec");
}


} // namespace client
} // namespace whisper


