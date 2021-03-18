#include "clientcommandprocessor.h"

#include <packet.h>
#include <connection.h>

namespace whisper {
namespace client {

using namespace common;

#define CLIENT_HANDLER(commandId, vp, vc, cs) void ClientCommandProcessor::handle_##commandId(CommandProcessor* vp, Connection *vc, const SerializedCommand &serializedCommand, ConnectionState *cs)
#define DESERIALIZE(variable, commandId) const auto variable = serializedCommand.deserialize<common::commandId>()
#define INSERT_HANDLER(processor, commandId) processor->insertHandler(common::command::commandId, &ClientCommandProcessor::handle_##commandId)
#define REMOVE_HANDLER(processor, commandId) processor->removeHandler(common::command::commandId)

ClientCommandProcessor::ClientCommandProcessor(QObject *parent)
    : CommandProcessor(parent)
{
    INSERT_HANDLER(this, SC_HANDSHAKE_REPLY);
    INSERT_HANDLER(this, SC_RETRY);
}

CLIENT_HANDLER(SC_HANDSHAKE_REPLY, p, c, s) {
    DESERIALIZE(cmd, SC_HANDSHAKE_REPLY);
    wDebug << cmd.handshakeReply_;
    c->send(CS_HANDSHAKE_SOLUTION{ "handshakeSolution solution" });
}

CLIENT_HANDLER(SC_RETRY, p, c, s) {
    DESERIALIZE(cmd, SC_RETRY);
}

} // namespace client
} // namespace whisper
