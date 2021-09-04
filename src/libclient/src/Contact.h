#pragma once

#include "libclient_export.h"

#include <QDateTime>

namespace whisper::client {

struct WHISPER_LIBCLIENT Contact {
    quint64 userId{ 0 };
    QByteArray certificate;
    bool approved{ false };
    QDateTime updateTimestamp;
};

} // namespace whisper::client
