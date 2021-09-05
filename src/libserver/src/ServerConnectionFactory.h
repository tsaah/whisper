#pragma once

#include "libserver_export.h"
#include "IServerConnectionFactory.h"

namespace whisper::server {

class WHISPER_LIBSERVER ServerConnectionFactory: public IServerConnectionFactory {
public:
    QTcpSocket* create(qintptr handle) override;
};

} // namespace whisper::server
