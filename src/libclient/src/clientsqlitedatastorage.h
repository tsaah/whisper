#ifndef CLIENTSQLITEDATASTORAGE_H
#define CLIENTSQLITEDATASTORAGE_H

#include "iclientdatastorage.h"

namespace whisper {
namespace client {

class WHISPER_LIBCLIENT ClientSqliteDataStorage: public IClientDataStorage {
    Q_OBJECT
public:
    explicit ClientSqliteDataStorage(QObject* parent = nullptr);

private:
    QSqlDatabase db() const;
    void initialize();
    void purge();
    void createTables();
};

} // namespace client
} // namespace whisper

#endif // CLIENTSQLITEDATASTORAGE_H
