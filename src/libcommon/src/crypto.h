#pragma once

#include "libcommon_export.h"

#include <QByteArray>

namespace whisper::common {

class PrivateKey;
class PublicKey;

class WHISPER_LIBCOMMON Crypto {
public:
    static QByteArray generateRandomByteArray(int size);
    static QByteArray generateSalt();
    static QByteArray encryptAES(const QByteArray& source, const QByteArray& key);
    static QByteArray decryptAES(const QByteArray& source, const QByteArray& key);
    static QByteArray encryptRSA(const QByteArray& source, const PublicKey& publicKey);
    static QByteArray decryptRSA(const QByteArray& source, const PrivateKey& privateKey);
    static QByteArray signRSA(const QByteArray& source, const PrivateKey& privateKey);
    static bool verifyRSA(const QByteArray& source, const PublicKey& publicKey, const QByteArray& signature);
};

} // namespace whisper::common
