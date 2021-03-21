#include "clientcommandprocessor.h"

#include <packet.h>
#include <connection.h>

namespace whisper {
namespace client {

using namespace common;

#define CLIENT_HANDLER(commandId, vp, vc, cs, ds) void ClientCommandProcessor::handle_##commandId(CommandProcessor* vp, Connection *vc, const SerializedCommand &serializedCommand, ConnectionState *cs, DataStorage* ds)
#define DESERIALIZE(variable, commandId) const auto variable = serializedCommand.deserialize<common::commandId>()
#define INSERT_HANDLER(processor, commandId) processor->insertHandler(common::command::commandId, &ClientCommandProcessor::handle_##commandId)
#define REMOVE_HANDLER(processor, commandId) processor->removeHandler(common::command::commandId)
#define CAST_PROCESSOR(from, to) auto* to = reinterpret_cast<ClientCommandProcessor*>(from)

ClientCommandProcessor::ClientCommandProcessor(QObject *parent)
    : CommandProcessor(parent)
{
    reset();
}

void ClientCommandProcessor::reset() {
    clear();
    INSERT_HANDLER(this, SC_HANDSHAKE_SUCCESSFULL);
    INSERT_HANDLER(this, SC_HANDSHAKE_REPLY);
}

CLIENT_HANDLER(SC_HANDSHAKE_REPLY, p, c, s, d) {
    DESERIALIZE(cmd, SC_HANDSHAKE_REPLY);
    REMOVE_HANDLER(p, SC_HANDSHAKE_REPLY);
    INSERT_HANDLER(p, SC_HANDSHAKE_RETRY);
    CAST_PROCESSOR(p, proc);
    emit proc->handshakeChallenge(cmd.handshakeReply);
    wDebug << cmd.handshakeReply;
}

CLIENT_HANDLER(SC_HANDSHAKE_RETRY, p, c, s, d) {
    wDebug;
    CAST_PROCESSOR(p, proc);
    emit proc->handshakeRetry();
//    c->send(CS_HANDSHAKE_SOLUTION{ "123" });
    wDebug << "wrong challenge response";
}

CLIENT_HANDLER(SC_HANDSHAKE_SUCCESSFULL, p, c, s, d) {
    wDebug;
    CAST_PROCESSOR(p, proc);
    emit proc->handshakeSuccessfull();
}

} // namespace client
} // namespace whisper


