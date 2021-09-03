#pragma once

#include "libcommon_export.h"

#include <QByteArray>

namespace whisper::common::crypto {

using RSA_TYPE = void;

class WHISPER_LIBCOMMON PrivateKey {
public:
    PrivateKey() = default;
    PrivateKey(const QByteArray& key);
    ~PrivateKey();

    bool generate();
    QByteArray toByteArray() const;
    bool fromByteArray(const QByteArray& key);
    void clear();
    RSA_TYPE* get() const;

private:
    RSA_TYPE* key_{ nullptr };
};

} // namespace whisper::common::crypto
