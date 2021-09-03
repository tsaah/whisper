#pragma once

#include "libcommon_export.h"
#include "log.h"

#include <QObject>

namespace whisper::common {

class WHISPER_LIBCOMMON DataStorage: public QObject {
    Q_OBJECT
public:
    explicit DataStorage(QObject* parent = nullptr): QObject(parent) {}
    virtual ~DataStorage() = default;
};

} // namespace whisper::common

#define SQLASSERT(v, s) wError << (v) << s;
#define SQLDBASSERT(v, s) wError << (v) << s;
