#ifndef CRYPTO_H
#define CRYPTO_H

#include "libcommon_export.h"

#include <QByteArray>
#include <QPair>

namespace whisper {
namespace common {

class WHISPER_LIBCOMMON Crypto {
public:
    static QByteArray generateNewDeviceCertificate();
    static QPair<QByteArray, QByteArray> generateChallengeResponse();
};

} // namespace common
} // namespace whisper

#endif // CRYPTO_H
