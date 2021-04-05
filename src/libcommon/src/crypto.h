#ifndef CRYPTO_H
#define CRYPTO_H

#include "libcommon_export.h"

#include <QByteArray>
#include <QPair>

namespace whisper {
namespace common {

using Certificate = QByteArray;

class WHISPER_LIBCOMMON Crypto {
public:
    static Certificate generateNewDeviceCertificate();
    static Certificate generateNewUserCertificate();
    static QByteArray generateSalt();
    static QByteArray hashPassword(const QString& password, const QByteArray& salt);
    static QPair<QString, QString> generateChallengeResponse();
    static quint64 generateNewUserId();
};

} // namespace common
} // namespace whisper

#endif // CRYPTO_H
