#include "clientcommandprocessor.h"
#include "clientconnectionstate.h"
#include "clientsqlitedatastorage.h"

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
#define CAST_STATE(from, to) auto* to = reinterpret_cast<ClientConnectionState*>(from)
#define CAST_STORAGE(from, to) auto* to = reinterpret_cast<ClientSqliteDataStorage*>(from)

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
    p->clear();
    INSERT_HANDLER(p, SC_NEW_USER_CREATED);
    INSERT_HANDLER(p, SC_AUTHORIZED);
    emit proc->handshakeSuccessfull();
}

CLIENT_HANDLER(SC_NEW_USER_CREATED, p, c, s, d) {
    wDebug;
    DESERIALIZE(cmd, SC_NEW_USER_CREATED);
    CAST_PROCESSOR(p, proc);
    CAST_STATE(s, st);
    CAST_STORAGE(d, db);

    st->userId = cmd.userId;
    db->storeUserId(cmd.userId);
    emit proc->newUserId(cmd.userId);
}

CLIENT_HANDLER(SC_AUTHORIZED, p, c, s, d) {
    wDebug;
    CAST_PROCESSOR(p, proc);
    CAST_STATE(s, st);
    CAST_STORAGE(d, db);

    st->authorized = true;
    emit proc->authorized(true);

    INSERT_HANDLER(p, CC_ADD_CONTACT_REQUEST);
    INSERT_HANDLER(p, CC_ADD_CONTACT_ACCEPT);
    INSERT_HANDLER(p, CC_MESSAGE);
}

CLIENT_HANDLER(CC_ADD_CONTACT_REQUEST, p, c, s, d) {
    wDebug;
    DESERIALIZE(cmd, CC_ADD_CONTACT_REQUEST);
    CAST_PROCESSOR(p, proc);
    emit proc->contactRequest(cmd.userId);
}

CLIENT_HANDLER(CC_ADD_CONTACT_ACCEPT, p, c, s, d) {
    wDebug;
    DESERIALIZE(cmd, CC_ADD_CONTACT_ACCEPT);
    CAST_PROCESSOR(p, proc);
    // store contact as confirmed
    emit proc->contactAccepted(cmd.userId);
}

CLIENT_HANDLER(CC_MESSAGE, p, c, s, d) {
    wDebug;
    DESERIALIZE(cmd, CC_MESSAGE);
    CAST_PROCESSOR(p, proc);
    emit proc->incomingMessage(cmd.userId, cmd.encryptedMessage);
}

} // namespace client
} // namespace whisper


