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

} // namespace common
} // namespace whisper
