#ifndef ISERVERDATASTORAGE_H
#define ISERVERDATASTORAGE_H

#include "libserver_export.h"

#include <datastorage.h>

#include <QSqlDatabase>

namespace whisper {
namespace server {

class WHISPER_LIBSERVER IServerDataStorage: public common::DataStorage {
    Q_OBJECT
public:
    explicit IServerDataStorage(QObject* parent = nullptr): common::DataStorage(parent) {}

    virtual bool isDeviceKnown(const QByteArray& deviceCert) const = 0;
    virtual void rememberDevice(const QByteArray& deviceCert) = 0;
};

} // namespace server
} // namespace whisper

#endif // ISERVERDATASTORAGE_H
