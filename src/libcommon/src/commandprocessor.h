#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include <QObject>

namespace whisper {
namespace common {

class CommandProcessor: public QObject {
    Q_OBJECT
public:
    explicit CommandProcessor(QObject *parent = nullptr);
};

} // namespace common
} // namespace whisper

#endif // COMMANDPROCESSOR_H
