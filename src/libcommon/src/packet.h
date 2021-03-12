#ifndef PACKET_H
#define PACKET_H

#include "libcommon_export.h"

namespace whisper {
namespace common {

struct WHISPER_LIBCOMMON Packet {
    quint64 magic_;
    qint64 size_;
    QByteArray payload_;
};

} // namespace common
} // namespace whisper

#endif // PACKET_H
