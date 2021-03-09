#ifndef PACKET_H
#define PACKET_H

#include "common_export.h"

namespace whisper {
namespace network {

struct WHISPER_COMMON_LIB Packet {
    quint64 magic_;
    qint64 size_;
    QByteArray payload_;
};

} // namespace network
} // namespace whisper

#endif // PACKET_H
