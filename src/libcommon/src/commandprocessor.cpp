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

void CommandProcessor::processCommand(Connection* connection, const SerializedCommand& serializedCommand, ConnectionState* state, DataStorage* storage) {
    if (handlers_.contains(serializedCommand.id_)) {
        handlers_[serializedCommand.id_](this, connection, serializedCommand, state, storage);
    } else {
        wDebug << "wrong command";
        onWrongCommand(connection);
    }
}

void CommandProcessor::clear() {
    handlers_.clear();
}

void CommandProcessor::onWrongCommand(Connection* connection) {

}

} // namespace common
} // namespace whisper

#define ADD_COMMAND_HANDLER(commandId) insertHandler(command::commandId, [](Connection* connection, const SerializedCommand& serializedCommand, ConnectionState* state, DataStorage* storage)
#define DESERIALIZE_COMMAND(commandId, variableName) const auto variableName = serializedCommand.deserialize<commandId>();
