#ifndef CONNECTIONSTATE_H
#define CONNECTIONSTATE_H

#include "types.h"
#include "libcommon_export.h"

#include <QObject>

namespace whisper {
namespace common {

class WHISPER_LIBCOMMON ConnectionState: public QObject {
    Q_OBJECT
public:
    explicit ConnectionState(QObject* parent = nullptr): QObject(parent) {}
    virtual ~ConnectionState() = default;
};

} // namespace common
} // namespace whisper

#endif // CONNECTIONSTATE_H
