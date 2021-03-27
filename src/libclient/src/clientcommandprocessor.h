#ifndef CLIENTCOMMANDPROCESSOR_H
#define CLIENTCOMMANDPROCESSOR_H

#include "libclient_export.h"

#include <commandprocessor.h>

namespace whisper {
namespace client {

#define DECLARE_CLIENT_HANDLER(commandId) static void handle_##commandId(common::CommandProcessor* processor, common::Connection* connection, const common::SerializedCommand& serializedCommand, common::ConnectionState* state, common::DataStorage* storage)

class WHISPER_LIBCLIENT ClientCommandProcessor: public common::CommandProcessor {
    Q_OBJECT
public:
    ClientCommandProcessor(QObject* parent = nullptr);
    void reset();

private:
    DECLARE_CLIENT_HANDLER(SC_HANDSHAKE_REPLY);
    DECLARE_CLIENT_HANDLER(SC_HANDSHAKE_RETRY);
    DECLARE_CLIENT_HANDLER(SC_HANDSHAKE_SUCCESSFULL);
    DECLARE_CLIENT_HANDLER(SC_NEW_USER_CREATED);
    DECLARE_CLIENT_HANDLER(SC_AUTHORIZED);

    DECLARE_CLIENT_HANDLER(CC_ADD_CONTACT_REQUEST);
    DECLARE_CLIENT_HANDLER(CC_ADD_CONTACT_ACCEPT);
    DECLARE_CLIENT_HANDLER(CC_MESSAGE);

public:
signals:
    void handshakeChallenge(QString);
    void handshakeRetry();
    void handshakeSuccessfull();
    void authorized(bool);
    void newUserId(quint64);
    void contactRequest(quint64);
    void contactAccepted(quint64);
    void incomingMessage(quint64, QByteArray);

};

} // namespace client
} // namespace whisper

#endif // CLIENTCOMMANDPROCESSOR_H
