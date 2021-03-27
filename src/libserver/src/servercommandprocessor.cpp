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
    REMOVE_HANDLER(p, CS_HANDSHAKE_REQUEST); // as soon as we get handshake request remove it from handlers
    CAST_STATE(s, state);
    CAST_STORAGE(d, ds);
    DESERIALIZE(cmd, CS_HANDSHAKE_REQUEST);
    state->deviceCertificate = cmd.deviceCertificate;
    // check that device certificate is correct
    if (ds->deviceExists(state->deviceCertificate)) {
        p->clear();
        INSERT_HANDLER(p, CS_NEW_USER);
//        INSERT_HANDLER(p, CS_OLD_USER);
        c->send(SC_HANDSHAKE_SUCCESSFULL{});
        c->stopKillTimer();
        wDebug << "successfull";
    } else {
        INSERT_HANDLER(p, CS_HANDSHAKE_SOLUTION);
        if (state->retryCount_ == 0) {
            const auto cr = Crypto::generateChallengeResponse();
            state->solutionHint = cr.first;
            state->expectedSolution = cr.second;
            wInfo << cr.first << ":" << cr.second;
        }
        c->send(SC_HANDSHAKE_REPLY{ state->solutionHint });
    }
}

SERVER_HANDLER(CS_HANDSHAKE_SOLUTION, p, c, s, d) {
    DESERIALIZE(cmd, CS_HANDSHAKE_SOLUTION);
    CAST_STATE(s, state);
    if (cmd.handshakeSolution == state->expectedSolution) {
        state->handshakeSuccessfull = true;
        p->clear();
        INSERT_HANDLER(p, CS_NEW_USER);
//        INSERT_HANDLER(p, CS_OLD_USER);
//        INSERT_HANDLER(p, CS_INTERACTIVE_CHALLENGE_REPLY);
        CAST_STORAGE(d, ds);
        ds->rememberDevice(state->deviceCertificate);
        c->send(SC_HANDSHAKE_SUCCESSFULL{});
        c->stopKillTimer();
        wDebug << "successfull";
    } else {
        if (state->retryCount_++ > 3) { // TODO: get rid of magic numbers
            c->close();
            c->deleteLater();
            wDebug << "failed";
        } else {
            c->send(SC_HANDSHAKE_RETRY{});
            wDebug << "retry" << state->retryCount_;
        }
    }
}

SERVER_HANDLER(CS_NEW_USER, p, c, s, d) {
    wDebug;
    DESERIALIZE(cmd, CS_NEW_USER);
    CAST_STORAGE(d, db);
    if (db->userExists(cmd.userCertificate)) {
        c->close();
        c->deleteLater();
        wDebug << "user certificate is already known";
    } else {
        quint64 userId = 0;
        do {
            userId = Crypto::generateNewUserId();
        } while (db->userIdExists(userId));
        const auto salt = Crypto::generateSalt();
        const auto passwordHash = Crypto::hashPassword(cmd.password, salt);
        CAST_STATE(s, st);
        db->rememberNewUser(userId, cmd.userCertificate, st->deviceCertificate, passwordHash, salt);
        st->userCertificate = cmd.userCertificate;
        st->userId = userId;
        st->userAuthorized = true;
        c->send(SC_NEW_USER_CREATED{ userId });
        c->send(SC_AUTHORIZED{});
        p->clear();
        // add message handlers
    }
}

SERVER_HANDLER(CS_OLD_USER, p, c, s, d) {
    wDebug;
    DESERIALIZE(cmd, CS_OLD_USER);
    CAST_STORAGE(d, db);
    CAST_STATE(s, st);
    if (db->userIdExists(cmd.userId)) {
        // TODO: wipe password
        // TODO: safe password
        const auto storedPasswordHash = db->getPasswordHash(cmd.userId);
        const auto salt = db->getPasswordSalt(cmd.userId);
        const auto passwordHash = Crypto::hashPassword(cmd.password, salt);
        if (storedPasswordHash == passwordHash) {
            const bool deviceIsKnown = db->deviceExists(st->deviceCertificate);
            if (deviceIsKnown) {
                st->userCertificate = db->getUserCertificate(cmd.userId);
                st->userId = cmd.userId;
                st->userAuthorized = true;
            } else {
                // TODO: new device routine
            }
            c->send(SC_AUTHORIZED{});
            p->clear();
            // add message handlers
        } else {
            // TODO: several attempts support
            c->close();
            c->deleteLater();
            wDebug << "fail wrong pass";
        }
    } else {
        c->close();
        c->deleteLater();
        wDebug << "fail we don't know u";
    }
}

SERVER_HANDLER(CS_INTERACTIVE_CHALLENGE_REPLY, p, c, s, d) {
    wDebug;
    DESERIALIZE(cmd, CS_INTERACTIVE_CHALLENGE_REPLY);
}

SERVER_HANDLER(CC_ADD_CONTACT_REQUEST, p, c, s, d) {
    wDebug;
    DESERIALIZE(cmd, CC_ADD_CONTACT_REQUEST);
    // enqueue contact request for user from cmd

    // TODO: dispatcher->enqueueContactRequest(userId);
}

SERVER_HANDLER(CC_ADD_CONTACT_ACCEPT, p, c, s, d) {
    wDebug;
    DESERIALIZE(cmd, CC_ADD_CONTACT_ACCEPT);
    // enqueue contact accepted reply for user from cmd
    // TODO: dispatcher->enqueueContactRequestAccepted(userId);
}

SERVER_HANDLER(CC_MESSAGE, p, c, s, d) {
    wDebug;
    DESERIALIZE(cmd, CC_MESSAGE);
    // enqueue message for user from cmd
    // TODO: dispatcher->enqueueMessage(userId, message);
}

} // namespace server
} // namespace whisper

