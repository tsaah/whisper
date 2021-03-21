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

QSqlDatabase ClientSqliteDataStorage::db() const {
    if (!QSqlDatabase::contains("data_storage.sqlite")) {
        auto db = QSqlDatabase::addDatabase("QSQLITE", "data_storage.sqlite");
        db.setDatabaseName("data_storage.sqlite");
    }
    return QSqlDatabase::database("data_storage.sqlite");
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
//    QSqlQuery q(db());
//    const bool ok = q.exec(R"(
//            CREATE TABLE IF NOT EXISTS `device_table` (
//                `hash` INTEGER PRIMARY KEY UNIQUE NOT NULL,
//                `certificate` BLOB UNIQUE NOT NULL
//            );
//        )");
//    Q_ASSERT_X(ok, __FUNCTION__, "query exec");
}


} // namespace client
} // namespace whisper
