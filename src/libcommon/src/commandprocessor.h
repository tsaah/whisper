#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include "packet.h"
#include "types.h"
#include "connectionstate.h"

#include <QByteArray>
#include <QHash>
#include <QObject>

#include <functional>

namespace whisper {
namespace common {

class Connection;
class CommandProcessor;

using Handler = std::function<void(CommandProcessor*, Connection*, const SerializedCommand&, ConnectionState*)>;

class WHISPER_LIBCOMMON CommandProcessor: public QObject {
    Q_OBJECT
public:
    explicit CommandProcessor(QObject* parent = nullptr);

    void insertHandler(CommandId id, Handler handler);
    void removeHandler(CommandId id);
    void processCommand(Connection* connection, const SerializedCommand& serializedCommand, ConnectionState* state);

private:
    QHash<CommandId, Handler> handlers_;
};

} // namespace common
} // namespace whisper

#endif // COMMANDPROCESSOR_H
