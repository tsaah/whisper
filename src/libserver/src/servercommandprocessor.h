#ifndef SERVERCOMMANDPROCESSOR_H
#define SERVERCOMMANDPROCESSOR_H

#include "libserver_export.h"

#include <commandprocessor.h>

namespace whisper {
namespace server {

#define DECLARE_SERVER_HANDLER(commandId) static void handle_##commandId(common::CommandProcessor* processor, common::Connection* connection, const common::SerializedCommand& serializedCommand, common::ConnectionState* state, common::DataStorage* storage)

class WHISPER_LIBSERVER ServerCommandProcessor: public common::CommandProcessor {
    Q_OBJECT
public:
    ServerCommandProcessor(QObject* parent = nullptr);
    void reset();

protected:
    void onWrongCommand(common::Connection *connection) override;

private:
    DECLARE_SERVER_HANDLER(CS_HANDSHAKE_REQUEST);
    DECLARE_SERVER_HANDLER(CS_HANDSHAKE_SOLUTION);
    DECLARE_SERVER_HANDLER(CS_NEW_USER);
    DECLARE_SERVER_HANDLER(CS_OLD_USER);
    DECLARE_SERVER_HANDLER(CS_INTERACTIVE_CHALLENGE_REPLY);

    DECLARE_SERVER_HANDLER(CC_ADD_CONTACT_REQUEST);
    DECLARE_SERVER_HANDLER(CC_ADD_CONTACT_ACCEPT);
    DECLARE_SERVER_HANDLER(CC_MESSAGE);
};

} // namespace server
} // namespace whisper



#endif // SERVERCOMMANDPROCESSOR_H
