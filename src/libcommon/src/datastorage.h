#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include "libcommon_export.h"

#include <QObject>

namespace whisper {
namespace common {

class WHISPER_LIBCOMMON DataStorage: public QObject {
    Q_OBJECT
public:
    explicit DataStorage(QObject* parent = nullptr): QObject(parent) {}
    virtual ~DataStorage() = default;
};

} // namespace common
} // namespace whisper

#endif // DATASTORAGE_H
