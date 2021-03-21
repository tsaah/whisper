#ifndef SERVERSQLITEDATASTORAGE_H
#define SERVERSQLITEDATASTORAGE_H

#include "iserverdatastorage.h"

namespace whisper {
namespace server {

class WHISPER_LIBSERVER ServerSqliteDataStorage: public IServerDataStorage {
    Q_OBJECT
public:
    explicit ServerSqliteDataStorage(QObject* parent = nullptr);

    bool isDeviceKnown(const QByteArray& deviceCert) const override;
    void rememberDevice(const QByteArray& deviceCert) override;

private:
    QSqlDatabase db() const;
    void initialize();
    void purge();
    void createTables();
};

} // namespace server
} // namespace whisper

#endif // SERVERSQLITEDATASTORAGE_H
