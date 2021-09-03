#include "PrivateKey.h"

#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/aes.h>
#include <openssl/rsa.h>

namespace whisper::common::crypto {

PrivateKey::PrivateKey(const QByteArray& key) {
    fromByteArray(key);
}
PrivateKey::~PrivateKey() {
    clear();
}
bool PrivateKey::generate() {
    clear();

    auto* bigNumber = BN_new();
    if (!bigNumber) {
        return false;
    }

    const auto initBigNumberResult = BN_set_word(bigNumber, RSA_F4);
    if (initBigNumberResult != 1) {
        BN_free(bigNumber);
        return false;
    }

    key_ = RSA_new();
    if (!key_) {
        BN_free(bigNumber);
        return false;
    }

    const auto generateRSAPrivateKeyResult = RSA_generate_key_ex(reinterpret_cast<RSA*>(key_), 4096, bigNumber, nullptr);
    if (generateRSAPrivateKeyResult != 1) {
        BN_free(bigNumber);
        clear();
        return false;
    }

    BN_free(bigNumber);
    return true;
}
QByteArray PrivateKey::toByteArray() const {
    if (!key_) {
        return {};
    }

    auto* bio = BIO_new(BIO_s_secmem());
    BIO_set_close(bio, BIO_NOCLOSE);
    if (!bio) {
        return {};
    }

    const auto storePrivateKeyResult = PEM_write_bio_RSAPrivateKey(bio, reinterpret_cast<RSA*>(key_), nullptr, NULL, 0, nullptr, nullptr);
    if (storePrivateKeyResult != 1) {
        BIO_free(bio);
        return {};
    }

    char* data = nullptr;
    const auto length = BIO_get_mem_data(bio, &data);
    const auto result = QByteArray::fromRawData(data, length);

    BIO_free(bio);
    return result;
}
bool PrivateKey::fromByteArray(const QByteArray& key) {
    clear();

    auto* bio = BIO_new_mem_buf(key.constData(), key.size());
    BIO_set_close(bio, BIO_NOCLOSE);
    if (!bio) {
        return false;
    }

    key_ = PEM_read_bio_RSAPrivateKey(bio, nullptr, nullptr, nullptr);
    if (!key_) {
        BIO_free(bio);
        return false;
    }

    BIO_free(bio);
    return true;
}
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
void PrivateKey::clear() {
    if (key_) {
        RSA_free(reinterpret_cast<RSA*>(key_));
        key_ = nullptr;
    }
}
RSA_TYPE* PrivateKey::get() const {
    return key_;
}

} // namespace whisper::common::crypto

