#pragma once

#include "libserver_export.h"

class QTcpSocket;

namespace whisper::server {

class WHISPER_LIBSERVER IServerConnectionFactory {
public:
    ~IServerConnectionFactory() = default;
    virtual QTcpSocket* create(qintptr handle) = 0;
};

} // namespace whisper::server
