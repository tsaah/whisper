#ifndef CRYPTO_H
#define CRYPTO_H

#include "libcommon_export.h"

#include <QByteArray>

namespace whisper {
namespace common {

class WHISPER_LIBCOMMON Crypto {
public:
    static QByteArray generateNewDeviceCertificate();
};

} // namespace common
} // namespace whisper

#endif // CRYPTO_H
