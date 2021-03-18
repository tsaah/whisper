#include "servercommandprocessor.h"

#include <packet.h>
#include <connection.h>

namespace whisper {
namespace server {

using namespace common;

#define SERVER_HANDLER(commandId, vp, vc, cs) void ServerCommandProcessor::handle_##commandId(CommandProcessor* vp, Connection *vc, const SerializedCommand &serializedCommand, ConnectionState *cs)
#define DESERIALIZE(variable, commandId) const auto variable = serializedCommand.deserialize<common::commandId>()
#define INSERT_HANDLER(processor, commandId) processor->insertHandler(common::command::commandId, &ServerCommandProcessor::handle_##commandId)
#define REMOVE_HANDLER(processor, commandId) processor->removeHandler(common::command::commandId)

ServerCommandProcessor::ServerCommandProcessor(QObject *parent)
    : CommandProcessor(parent)
{
    INSERT_HANDLER(this, CS_HANDSHAKE_REQUEST); // initially we expect only handshake request
}

SERVER_HANDLER(CS_HANDSHAKE_REQUEST, p, c, s) {
    REMOVE_HANDLER(p, CS_HANDSHAKE_REQUEST); // as soon as we get handshake request remove it from handlers
    INSERT_HANDLER(p, CS_HANDSHAKE_SOLUTION); // expect handdhake solution next
    // state should store retry count
    // if retry count is 0 then state should store an expected solution
    DESERIALIZE(cmd, CS_HANDSHAKE_REQUEST);
    // check and store device certificate related to current connection
    wDebug << cmd.deviceCertificate_ << cmd.versionMajor_ << cmd.versionMinor_ << cmd.versionBuild_;
    c->send(SC_HANDSHAKE_REPLY{ "handshakeReply solution hint" });
}

SERVER_HANDLER(CS_HANDSHAKE_SOLUTION, p, c, s) {
    DESERIALIZE(cmd, CS_HANDSHAKE_REQUEST);
}

SERVER_HANDLER(CS_NEW_USER, p, c, s) {
    DESERIALIZE(cmd, CS_HANDSHAKE_REQUEST);
}

SERVER_HANDLER(CS_OLD_USER, p, c, s) {
    DESERIALIZE(cmd, CS_HANDSHAKE_REQUEST);
}

SERVER_HANDLER(CS_INTERACTIVE_CHALLENGE_REPLY, p, c, s) {
    DESERIALIZE(cmd, CS_HANDSHAKE_REQUEST);
}

} // namespace server
} // namespace whisper
