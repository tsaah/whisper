#include "crypto.h"

#include "privatekey.h"
#include "publickey.h"

#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/aes.h>
#include <openssl/rsa.h>

namespace whisper {
namespace common {

QByteArray Crypto::generateRandomByteArray(int size) {
    QByteArray random(size, '\0');
    if (RAND_bytes((unsigned char*)random.data(), random.size()) != 1) {
        return {};
    }
    return random;
}

QByteArray Crypto::generateSalt() {
    return generateRandomByteArray(PKCS5_SALT_LEN);
}

QByteArray Crypto::encryptAES(const QByteArray& source, const QByteArray& key) {
    auto* cipher = EVP_aes_256_cbc();
    if (!cipher) {
        return {};
    }

    const auto derivedKeySize = EVP_CIPHER_key_length(cipher);
    const auto ivSize = EVP_CIPHER_iv_length(cipher);
    QByteArray derivedKeyIvBuffer(derivedKeySize + ivSize, '\0');


    auto* digest = EVP_sha256();
    if (!digest) {
        return {};
    }

    const auto keyDerivationResult = PKCS5_PBKDF2_HMAC(key.constData(), key.size(), nullptr, 0, 10000, digest, derivedKeySize + ivSize, (unsigned char*)derivedKeyIvBuffer.data());
    if (!keyDerivationResult) {
        return {};
    }

    const auto derivedKey = QByteArray::fromRawData(derivedKeyIvBuffer.constData(), derivedKeySize);
    const auto iv = QByteArray::fromRawData(derivedKeyIvBuffer.constData() + derivedKeySize, ivSize);

    auto* context = EVP_CIPHER_CTX_new();
    if (!context) {
        return {};
    }

    const auto encryptInitResult = EVP_EncryptInit_ex(context, cipher, nullptr, (const unsigned char*)derivedKey.constData(), (const unsigned char*)iv.constData());
    if (encryptInitResult != 1) {
        EVP_CIPHER_CTX_free(context);
        return {};
    }

    QByteArray result(source.size() + EVP_CIPHER_block_size(cipher) - 1, '\0');

    int encryptedLength = 0;
    int updateLength = 0;
    const auto encryptUpdateResult = EVP_EncryptUpdate(context, (unsigned char*)result.data(), &updateLength, (const unsigned char*)source.constData(), source.size());
    if (encryptUpdateResult != 1) {
        EVP_CIPHER_CTX_free(context);
        return {};
    }
    encryptedLength = updateLength;

    const auto encryptFinalResult = EVP_EncryptFinal_ex(context, (unsigned char*)result.data() + updateLength, &updateLength);
    if (encryptFinalResult != 1) {
        EVP_CIPHER_CTX_free(context);
        return {};
    }
    encryptedLength += updateLength;

    result.resize(encryptedLength);

    EVP_CIPHER_CTX_free(context);

    return result.toBase64();
}

QByteArray Crypto::decryptAES(const QByteArray& source, const QByteArray& key) {
    auto* cipher = EVP_aes_256_cbc();
    if (!cipher) {
        return {};
    }

    const auto derivedKeySize = EVP_CIPHER_key_length(cipher);
    const auto ivSize = EVP_CIPHER_iv_length(cipher);
    QByteArray derivedKeyIvBuffer(derivedKeySize + ivSize, '\0');


    auto* digest = EVP_sha256();
    if (!digest) {
        return {};
    }

    const auto keyDerivationResult = PKCS5_PBKDF2_HMAC(key.constData(), key.size(), nullptr, 0, 10000, digest, derivedKeySize + ivSize, (unsigned char*)derivedKeyIvBuffer.data());
    if (!keyDerivationResult) {
        return {};
    }

    const auto derivedKey = QByteArray::fromRawData(derivedKeyIvBuffer.constData(), derivedKeySize);
    const auto iv = QByteArray::fromRawData(derivedKeyIvBuffer.constData() + derivedKeySize, ivSize);

    auto* context = EVP_CIPHER_CTX_new();
    if (!context) {
        return {};
    }

    const auto decryptInitResult = EVP_DecryptInit_ex(context, cipher, nullptr, (const unsigned char*)derivedKey.constData(), (const unsigned char*)iv.constData());
    if (decryptInitResult != 1) {
        EVP_CIPHER_CTX_free(context);
        return {};
    }


    const auto decodedSource = QByteArray::fromBase64(source);
    QByteArray result(source.size() + EVP_CIPHER_block_size(cipher), '\0');

    int decryptedLength = 0;
    int updateLength = 0;
    const auto decryptUpdateResult = EVP_DecryptUpdate(context, (unsigned char*)result.data(), &updateLength, (const unsigned char*)decodedSource.constData(), decodedSource.size());
    if (decryptUpdateResult != 1) {
        EVP_CIPHER_CTX_free(context);
        return {};
    }
    decryptedLength = updateLength;


    const auto decryptFinalResult = EVP_DecryptFinal_ex(context, (unsigned char*)result.data() + updateLength, &updateLength);
    if (decryptFinalResult != 1) {
        EVP_CIPHER_CTX_free(context);
        return {};
    }
    decryptedLength += updateLength;

    result.resize(decryptedLength);

    EVP_CIPHER_CTX_free(context);

    return result;
}

QByteArray Crypto::encryptRSA(const QByteArray& source, const PublicKey& publicKey) {
    auto* context = EVP_CIPHER_CTX_new();
    if (!context) {
        return {};
    }

    auto* cipher = EVP_aes_256_cbc();
    if (!cipher) {
        return {};
    }

    auto* pKey = EVP_PKEY_new();
    if (!pKey) {
        EVP_CIPHER_CTX_free(context);
        return {};
    }
    EVP_PKEY_assign_RSA(pKey, reinterpret_cast<RSA*>(publicKey.get()));

    auto encryptedAESKeySize = EVP_PKEY_size(pKey);
    Q_ASSERT(encryptedAESKeySize == 512);
    const auto ivSize = EVP_CIPHER_iv_length(cipher);
    Q_ASSERT(ivSize == 16);

    QByteArray tek(encryptedAESKeySize, '\0');
    QByteArray tiv(ivSize, '\0');
    QByteArray ted(source.size() + EVP_CIPHER_block_size(cipher), '\0');

    unsigned char* encrypted_key = reinterpret_cast<unsigned char*>(tek.data());
    unsigned char* iv = reinterpret_cast<unsigned char*>(tiv.data());
    unsigned char* encryptedData = reinterpret_cast<unsigned char*>(ted.data());

    const auto sealInitResult = EVP_SealInit(context, cipher, &encrypted_key, &encryptedAESKeySize, iv, &pKey, 1);
    if (sealInitResult != 1) {
        EVP_CIPHER_CTX_free(context);
        return {};
    }

    Q_ASSERT(encryptedAESKeySize == 512);

    int encryptedLength = 0;
    int updateLength = 0;
    const auto encryptUpdateResult = EVP_EncryptUpdate(context, encryptedData, &updateLength, (const unsigned char*)source.constData(), source.size());
    if (encryptUpdateResult != 1) {
        EVP_CIPHER_CTX_free(context);
        return {};
    }
    encryptedLength = updateLength;


    const auto sealFinalResult = EVP_SealFinal(context, encryptedData + updateLength, &updateLength);
    if (sealFinalResult != 1) {
        EVP_CIPHER_CTX_free(context);
        return {};
    }
    encryptedLength += updateLength;
    ted.resize(encryptedLength);

    QByteArray result;
    result.append(tiv);
    result.append(tek);
    result.append(ted);

    EVP_CIPHER_CTX_free(context);

    return result.toBase64();
}

QByteArray Crypto::decryptRSA(const QByteArray& source, const PrivateKey& privateKey) {


    auto* pKey = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(pKey, reinterpret_cast<RSA*>(privateKey.get()));


    auto* context = EVP_CIPHER_CTX_new();
    if (!context) {
        return {};
    }


    auto* cipher = EVP_aes_256_cbc();
    if (!cipher) {
        return {};
    }

    auto encryptedAESKeySize = EVP_PKEY_size(pKey);
    Q_ASSERT(encryptedAESKeySize == 512);
    const auto ivSize = EVP_CIPHER_iv_length(cipher);

    const auto decodedSource = QByteArray::fromBase64(source);
    const auto tiv = QByteArray::fromRawData(decodedSource.constData(), ivSize);
    const auto tek = QByteArray::fromRawData(decodedSource.constData() + ivSize, encryptedAESKeySize);
    const auto ted = QByteArray::fromRawData(decodedSource.constData() + ivSize + encryptedAESKeySize, decodedSource.size() - ivSize - encryptedAESKeySize);

    const unsigned char* encrypted_key = reinterpret_cast<const unsigned char*>(tek.data());
    const unsigned char* iv = reinterpret_cast<const unsigned char*>(tiv.data());
    const unsigned char* encryptedData = reinterpret_cast<const unsigned char*>(ted.data());


    const auto decryptInitResult = EVP_OpenInit(context, cipher, encrypted_key, encryptedAESKeySize, iv, pKey);
    if (decryptInitResult != 1) {
        EVP_CIPHER_CTX_free(context);
        return {};
    }


    QByteArray result(1000 + source.size() + EVP_CIPHER_block_size(cipher), '\0');

    int decryptedLength = 0;
    int updateLength = 0;
    const auto decryptUpdateResult = EVP_DecryptUpdate(context, (unsigned char*)result.data(), &updateLength, encryptedData, ted.size());
    if (decryptUpdateResult != 1) {
        EVP_CIPHER_CTX_free(context);
        return {};
    }
    decryptedLength = updateLength;


    const auto decryptFinalResult = EVP_OpenFinal(context, (unsigned char*)result.data() + updateLength, &updateLength);
    if (decryptFinalResult != 1) {
        EVP_CIPHER_CTX_free(context);
        return {};
    }
    decryptedLength += updateLength;

    result.resize(decryptedLength);

    EVP_CIPHER_CTX_free(context);

    return result;
}

QByteArray Crypto::signRSA(const QByteArray& source, const PrivateKey& privateKey) {
    EVP_MD_CTX* m_RSASignCtx = EVP_MD_CTX_new();
    EVP_PKEY* priKey = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(priKey, reinterpret_cast<RSA*>(privateKey.get()));
    const auto digestSignInitResult = EVP_DigestSignInit(m_RSASignCtx, NULL, EVP_sha256(), NULL, priKey);
    if (digestSignInitResult <= 0) {
        return {};
    }
    if (EVP_DigestUpdate(m_RSASignCtx, source.constData(), source.size()) <= 0) {
        return {};
    }
    size_t MsgLenEnc = 0;
    if (EVP_DigestSignFinal(m_RSASignCtx, nullptr, &MsgLenEnc) <=0) {
        return {};
    }
    QByteArray e(MsgLenEnc, '\0');
    if (EVP_DigestSignFinal(m_RSASignCtx, (unsigned char*)e.data(), &MsgLenEnc) <= 0) {
        return {};
    }
    EVP_MD_CTX_free(m_RSASignCtx);
    return e.toBase64();
}

bool Crypto::verifyRSA(const QByteArray& source, const PublicKey& publicKey, const QByteArray& signature) {
    const auto decodedSignature = QByteArray::fromBase64(signature);

    EVP_PKEY* pubKey  = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(pubKey, reinterpret_cast<RSA*>(publicKey.get()));
    EVP_MD_CTX* m_RSAVerifyCtx = EVP_MD_CTX_new();

    if (EVP_DigestVerifyInit(m_RSAVerifyCtx, NULL, EVP_sha256(), NULL,pubKey) <= 0) {
        return false;
    }
    if (EVP_DigestUpdate(m_RSAVerifyCtx, source.constData(), source.size()) <= 0) {
        return false;
    }
    int AuthStatus = EVP_DigestVerifyFinal(m_RSAVerifyCtx, (const unsigned char*)decodedSignature.constData(), decodedSignature.size());
    if (AuthStatus == 1) {
        EVP_MD_CTX_free(m_RSAVerifyCtx);
        return true;
    } else{
        EVP_MD_CTX_free(m_RSAVerifyCtx);
        return false;
    }
}

} // namespace common
} // namespace whisper
