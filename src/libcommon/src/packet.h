#ifndef PACKET_H
#define PACKET_H

#include "libcommon_export.h"

namespace whisper {
namespace network {

struct WHISPER_LIBCOMMON Packet {
    quint64 magic_;
    qint64 size_;
    QByteArray payload_;
};

} // namespace network
} // namespace whisper

#endif // PACKET_H
