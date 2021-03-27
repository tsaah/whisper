#ifndef CLIENTCONNECTIONSTATE_H
#define CLIENTCONNECTIONSTATE_H

#include "libclient_export.h"

#include <connectionstate.h>

namespace whisper {
namespace client {

class WHISPER_LIBCLIENT ClientConnectionState: public common::ConnectionState {
    Q_OBJECT
public:
    ClientConnectionState(QObject* parent = nullptr): common::ConnectionState(parent) {}

    QByteArray deviceCertificate;
    QByteArray userCertificate;

    quint64 userId{ 0 };
    bool authorized{ false };
};

} // namespace client
} // namespace whisper

#endif // CLIENTCONNECTIONSTATE_H
