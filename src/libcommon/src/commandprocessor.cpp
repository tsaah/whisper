#include "commandprocessor.h"
#include "connection.h"
#include "log.h"

namespace whisper {
namespace common {

CommandProcessor::CommandProcessor(QObject* parent)
    : QObject(parent)
{
}

void CommandProcessor::insertHandler(CommandId id, Handler handler) {
    handlers_.insert(id, handler);
}

void CommandProcessor::removeHandler(CommandId id) {
    if (handlers_.contains(id)) {
        handlers_.remove(id);
    }
}

void CommandProcessor::processCommand(Connection* connection, const SerializedCommand& serializedCommand, ConnectionState* state) {
    if (handlers_.contains(serializedCommand.id_)) {
        handlers_[serializedCommand.id_](connection, serializedCommand, state);
    } else {
        wDebug << "wrong command";
        connection->close();
        connection->deleteLater();
    }
}

} // namespace common
} // namespace whisper

#define ADD_COMMAND_HANDLER(commandId) insertHandler(command::commandId, [](Connection* connection, const SerializedCommand& serializedCommand, ConnectionState* state)
#define DESERIALIZE_COMMAND(commandId, variableName) const auto variableName = serializedCommand.deserialize<commandId>();
