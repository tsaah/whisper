#pragma once

#include "libcommon_export.h"

#include <QByteArray>

namespace whisper::common {

using RSA_TYPE = void;

class WHISPER_LIBCOMMON PrivateKey {
public:
    PrivateKey() = default;
    PrivateKey(const QByteArray& key);
    ~PrivateKey();
    bool generate();
    QByteArray toByteArray() const;
    bool fromByteArray(const QByteArray& key);
//    PublicKey extractPublicKey() const {
//        if (!key_) {
//            return {};
//        }

//        auto* bio = BIO_new(BIO_s_secmem());
//        BIO_set_close(bio, BIO_NOCLOSE);
//        if (!bio) {
//            return {};
//        }

//        const auto storePublicKeyResult = PEM_write_bio_RSAPublicKey(bio, key_);
//        if (storePublicKeyResult != 1) {
//            BIO_free(bio);
//            return {};
//        }

//        char* data = nullptr;
//        const auto length = BIO_get_mem_data(bio, &data);
//        const auto result = QByteArray::fromRawData(data, length);

//        BIO_free(bio);
//        return { result };
//    }
    void clear();
    RSA_TYPE* get() const;

private:
    RSA_TYPE* key_{ nullptr };
};

} // namespace whisper::common
