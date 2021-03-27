#include "crypto.h"
#include <QHash>

namespace whisper {
namespace common {

QByteArray Crypto::generateNewDeviceCertificate() {
    QByteArray ba;
    for (int i = 0; i < 4096; ++i) {
        ba.append(static_cast<char>(qrand()));
    }
    return ba.toBase64();
}

QByteArray Crypto::generateNewUserCertificate() {
    QByteArray ba;
    for (int i = 0; i < 4096; ++i) {
        ba.append(static_cast<char>(qrand()));
    }
    return ba.toBase64();
}

QByteArray Crypto::generateSalt() {
    QByteArray r;
    for (int i = 0; i < 19; ++i) {
        r.append(QByteArray::number(qrand() % 10));
    }
    return r;
}

QByteArray Crypto::hashPassword(const QByteArray &password, const QByteArray &salt) {
    const auto saltedPassword = salt + password;
    return QByteArray::number(qHash(saltedPassword));
}

QPair<QByteArray, QByteArray> Crypto::generateChallengeResponse() {
    QByteArray c;
    QByteArray r;
    c = "enter me secret key to connect";
    for (int i = 0; i < 6; ++i) {
        r.append(QByteArray::number(qrand() % 10));
    }
    return { c, r };
}

quint64 Crypto::generateNewUserId()
{
    return static_cast<quint64>(qrand() % 1000000);
}

} // namespace common
} // namespace whisper
