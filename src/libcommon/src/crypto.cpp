#include "crypto.h"

namespace whisper {
namespace common {

QByteArray Crypto::generateNewDeviceCertificate() {
    QByteArray ba;
    for (int i = 0; i < 4096; ++i) {
        ba.append(static_cast<char>(qrand()));
    }
    return ba.toBase64();
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

} // namespace common
} // namespace whisper
