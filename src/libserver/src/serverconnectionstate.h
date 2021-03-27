#ifndef SERVERCONNECTIONSTATE_H
#define SERVERCONNECTIONSTATE_H

#include "libserver_export.h"

#include <connectionstate.h>

namespace whisper {
namespace server {

class WHISPER_LIBSERVER ServerConnectionState: public common::ConnectionState {
    Q_OBJECT
public:
    explicit ServerConnectionState(QObject* parent = nullptr): common::ConnectionState(parent) {}

    quint8 retryCount_{ 0 };
    QByteArray deviceCertificate;
    QByteArray expectedSolution;
    QByteArray solutionHint;
    bool handshakeSuccessfull{ false };
    QByteArray userCertificate;
    quint64 userId{ 0 };
    bool userAuthorized{ false };
};

} // namespace server
} // namespace whisper

#endif // SERVERCONNECTIONSTATE_H
