#include "serversqlitedatastorage.h"

#include <types.h>

#include <QHash>
#include <QSqlQuery>
#include <QVariant>

namespace whisper {
namespace server {

ServerSqliteDataStorage::ServerSqliteDataStorage(QObject *parent): IServerDataStorage(parent) {
    initialize();
}

bool ServerSqliteDataStorage::isDeviceKnown(const QByteArray &deviceCert) const {
    const auto hash = qHash(deviceCert);
    QSqlQuery q(db());
    bool ok = false;
    ok = q.prepare("SELECT EXISTS(SELECT 1 FROM `device_table` WHERE `hash` = :hash LIMIT 1);");
    Q_ASSERT_X(ok, __FUNCTION__, "query preparation");
    q.bindValue(":hash", hash);
    ok = q.exec();
    Q_ASSERT_X(ok, __FUNCTION__, "query exec");
    ok = q.next();
    Q_ASSERT_X(ok, __FUNCTION__, "query next");
    return q.value(0).toInt() == 1;
}

void ServerSqliteDataStorage::rememberDevice(const QByteArray &deviceCert) {
    const auto hash = qHash(deviceCert);
    QSqlQuery q(db());
    bool ok = false;
    ok = q.prepare("INSERT OR REPLACE INTO `device_table` (`hash`, `certificate`) VALUES (:hash, :cert);");
    Q_ASSERT_X(ok, __FUNCTION__, "query preparation");
    q.bindValue(":hash", hash);
    q.bindValue(":cert", deviceCert);
    ok = q.exec();
    Q_ASSERT_X(ok, __FUNCTION__, "query exec");
}

QSqlDatabase ServerSqliteDataStorage::db() const {
    if (!QSqlDatabase::contains("data_storage.sqlite")) {
        auto db = QSqlDatabase::addDatabase("QSQLITE", "data_storage.sqlite");
        db.setDatabaseName("data_storage.sqlite");
    }
    return QSqlDatabase::database("data_storage.sqlite");
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
    const bool ok = q.exec(R"(
            CREATE TABLE IF NOT EXISTS `device_table` (
                `hash` INTEGER PRIMARY KEY UNIQUE NOT NULL,
                `certificate` BLOB UNIQUE NOT NULL
            );
        )");
    Q_ASSERT_X(ok, __FUNCTION__, "query exec");
}

} // namespace server
} // namespace whisper
