#include "PublicKey.h"
#include "PrivateKey.h"

#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/aes.h>
#include <openssl/rsa.h>

namespace whisper::common::crypto {

PublicKey::PublicKey(const QByteArray& key) {
    fromByteArray(key);
}

PublicKey::PublicKey(const PrivateKey& key) {
    if (!key.get()) {
        return;
    }

    auto* bio = BIO_new(BIO_s_secmem());
    BIO_set_close(bio, BIO_NOCLOSE);
    if (!bio) {
        return;
    }

    const auto storePublicKeyResult = PEM_write_bio_RSAPublicKey(bio, reinterpret_cast<RSA*>(key.get()));
    if (storePublicKeyResult != 1) {
        BIO_free(bio);
        return;
    }

    char* data = nullptr;
    const auto length = BIO_get_mem_data(bio, &data);
    const auto result = QByteArray::fromRawData(data, length);

    fromByteArray(result);

    BIO_free(bio);
}

PublicKey::~PublicKey() {
    clear();
}

bool PublicKey::fromByteArray(const QByteArray& key) {
    clear();

    auto* bio = BIO_new_mem_buf(key.constData(), key.size());
    BIO_set_close(bio, BIO_NOCLOSE);
    if (!bio) {
        return false;
    }

    key_ = PEM_read_bio_RSAPublicKey(bio, nullptr, nullptr, nullptr);
    if (!key_) {
        BIO_free(bio);
        return false;
    }

    pem_ = key;
    BIO_free_all(bio);

    return true;
}

QByteArray PublicKey::toByteArray() const {
    return pem_;
}

void PublicKey::clear() {
    if (key_) {
        RSA_free(reinterpret_cast<RSA*>(key_));
    }
    pem_.fill('\0');
    pem_.clear();
}

RSA_TYPE *PublicKey::get() const {
    return key_;
}

bool PublicKey::isValid() const
{
    // TODO: key validation
    return key_ != nullptr;
}

} // namespace whisper::common::crypto
