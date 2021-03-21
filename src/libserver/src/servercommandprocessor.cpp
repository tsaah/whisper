#include "servercommandprocessor.h"
#include "serverconnectionstate.h"
#include "serversqlitedatastorage.h"

#include <packet.h>
#include <connection.h>
#include <datastorage.h>
#include <crypto.h>

namespace whisper {
namespace server {

using namespace common;

#define SERVER_HANDLER(commandId, vp, vc, cs, ds) void ServerCommandProcessor::handle_##commandId(CommandProcessor* vp, Connection *vc, const SerializedCommand &serializedCommand, ConnectionState *cs, common::DataStorage* ds)
#define DESERIALIZE(variable, commandId) const auto variable = serializedCommand.deserialize<common::commandId>()
#define INSERT_HANDLER(processor, commandId) processor->insertHandler(common::command::commandId, &ServerCommandProcessor::handle_##commandId)
#define REMOVE_HANDLER(processor, commandId) processor->removeHandler(common::command::commandId)
#define CAST_STATE(from, to) auto* to = reinterpret_cast<ServerConnectionState*>(from)
#define CAST_STORAGE(from, to) auto* to = reinterpret_cast<ServerSqliteDataStorage*>(from)

ServerCommandProcessor::ServerCommandProcessor(QObject *parent)
    : CommandProcessor(parent)
{
    reset();
}

void ServerCommandProcessor::reset() {
    clear();
    INSERT_HANDLER(this, CS_HANDSHAKE_REQUEST); // initially we expect only handshake request
}

void ServerCommandProcessor::onWrongCommand(Connection* connection) {
    connection->close();
    connection->deleteLater();
}

SERVER_HANDLER(CS_HANDSHAKE_REQUEST, p, c, s, d) {
    wDebug;
    REMOVE_HANDLER(p, CS_HANDSHAKE_REQUEST); // as soon as we get handshake request remove it from handlers
    CAST_STATE(s, state);
    CAST_STORAGE(d, ds);
    DESERIALIZE(cmd, CS_HANDSHAKE_REQUEST);
    state->deviceCertificate_ = cmd.deviceCertificate;

    // check that device certificate is correct
    if (ds->isDeviceKnown(state->deviceCertificate_)) {
        INSERT_HANDLER(p, CS_NEW_USER);
        INSERT_HANDLER(p, CS_OLD_USER);
        c->send(SC_HANDSHAKE_SUCCESSFULL{});
    } else { // run chllenge if not
        INSERT_HANDLER(p, CS_HANDSHAKE_SOLUTION); // expect handdhake solution next
        if (state->retryCount_ == 0) {
            const auto cr = Crypto::generateChallengeResponse();
            state->solutionHint_ = cr.first;
            state->expectedSolution_ = cr.second;
            wInfo << cr.first << ":" << cr.second;
        }
        c->send(SC_HANDSHAKE_REPLY{ state->solutionHint_ });
    }
}

SERVER_HANDLER(CS_HANDSHAKE_SOLUTION, p, c, s, d) {
    wDebug;
    DESERIALIZE(cmd, CS_HANDSHAKE_SOLUTION);
    CAST_STATE(s, state);
    if (cmd.handshakeSolution == state->expectedSolution_) {
        state->handshakeSuccessfull_ = true;
        REMOVE_HANDLER(p, CS_HANDSHAKE_SOLUTION);
        INSERT_HANDLER(p, CS_NEW_USER);
        INSERT_HANDLER(p, CS_OLD_USER);
        INSERT_HANDLER(p, CS_INTERACTIVE_CHALLENGE_REPLY);
        CAST_STORAGE(d, ds);
        ds->rememberDevice(state->deviceCertificate_);
        c->send(SC_HANDSHAKE_SUCCESSFULL{});
    } else {
        if (state->retryCount_++ > 3) { // TODO: get rid of magic numbers
            c->close();
            c->deleteLater();
        } else {
            c->send(SC_HANDSHAKE_RETRY{});
        }
    }
}

SERVER_HANDLER(CS_NEW_USER, p, c, s, d) {
    wDebug;
    DESERIALIZE(cmd, CS_NEW_USER);
}

SERVER_HANDLER(CS_OLD_USER, p, c, s, d) {
    wDebug;
    DESERIALIZE(cmd, CS_OLD_USER);
}

SERVER_HANDLER(CS_INTERACTIVE_CHALLENGE_REPLY, p, c, s, d) {
    wDebug;
    DESERIALIZE(cmd, CS_INTERACTIVE_CHALLENGE_REPLY);
}

} // namespace server
} // namespace whisper

