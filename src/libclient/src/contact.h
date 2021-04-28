#ifndef CONTACT_H
#define CONTACT_H

#include "libclient_export.h"

#include <QDateTime>

namespace whisper {
namespace client {

struct WHISPER_LIBCLIENT Contact {
    quint64 userId{ 0 };
    QByteArray certificate;
    bool approved{ false };
    QDateTime updateTimestamp;
};

} // namespace client
} // namespace whisper

#endif // CONTACT_H
