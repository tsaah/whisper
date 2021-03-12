#ifndef CLIENTPROCESSOR_H
#define CLIENTPROCESSOR_H

#include "libclient_export.h"

#include <QObject>

namespace whisper {
namespace client {

class WHISPER_LIBCLIENT ClientProcessor: public QObject {
    Q_OBJECT
public:
    explicit ClientProcessor(QObject *parent = nullptr);
};

} // namespace client
} // namespace whisper

#endif // CLIENTPROCESSOR_H
