#ifndef CLIENTPROCESSOR_H
#define CLIENTPROCESSOR_H

#include "client_export.h"

#include <QObject>

namespace whisper {
namespace network {

class WHISPER_CLIENT_LIB ClientProcessor: public QObject {
    Q_OBJECT
public:
    explicit ClientProcessor(QObject *parent = nullptr);
};

} // namespace network
} // namespace whisper

#endif // CLIENTPROCESSOR_H
