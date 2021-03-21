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

public:
signals:
    void handshakeChallenge(QString);
    void handshakeRetry();

};

} // namespace client
} // namespace whisper

#endif // CLIENTCOMMANDPROCESSOR_H
