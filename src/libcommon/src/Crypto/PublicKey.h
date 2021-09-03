#pragma once

#include "libcommon_export.h"

#include <QByteArray>

namespace whisper::common::crypto {

using RSA_TYPE = void;

class PrivateKey;

class WHISPER_LIBCOMMON PublicKey {
public:
    PublicKey() = default;
    PublicKey(const QByteArray& key);
    PublicKey(const PrivateKey& key);
    ~PublicKey();

    bool fromByteArray(const QByteArray& key);
    QByteArray toByteArray() const;
    void clear();
    RSA_TYPE* get() const;
    bool isValid() const;

private:
    RSA_TYPE* key_{ nullptr };
    QByteArray pem_;
};

} // namespace whisper::common::crypto
