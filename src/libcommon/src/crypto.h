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
    static QByteArray generateNewUserCertificate();
    static QByteArray generateSalt();
    static QByteArray hashPassword(const QByteArray& password, const QByteArray& salt);
    static QPair<QByteArray, QByteArray> generateChallengeResponse();
    static quint64 generateNewUserId();
};

} // namespace common
} // namespace whisper

#endif // CRYPTO_H
