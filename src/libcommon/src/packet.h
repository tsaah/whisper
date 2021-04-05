#ifndef PACKET_H
#define PACKET_H

#include "types.h"
#include "libcommon_export.h"
#include "stringhash.h"
#include "log.h"
#include "crypto.h"

#include <QDataStream>
#include <QMetaObject>
#include <QMetaProperty>

namespace whisper {
namespace common {

struct WHISPER_LIBCOMMON EncryptedCommand {
    CommandId id_;
    QByteArray data_;
};

struct WHISPER_LIBCOMMON SerializedCommand {
    CommandId id_;
    QByteArray data_;
    template<class Command>
    Command deserialize() const {
        Command result;
        QDataStream stream(data_);
        stream.setVersion(QDataStream::Qt_5_15);
        const auto propertyCount = Command::staticMetaObject.propertyCount();
        for (int i = 0; i < propertyCount; ++i) {
            auto property = Command::staticMetaObject.property(i);
            QVariant value;
            stream >> value;
            property.writeOnGadget(&result, value);
        }
        return result;
    }
};

//EncryptedCommand encrypt(const SerializedCommand& cmd) {
//    return { cmd.id_, cmd.data_ };
//}

//SerializedCommand decrypt(const EncryptedCommand& cmd) {
//    return { cmd.id_, cmd.data_ };
//}

namespace command {

#define COMMAND_ID(commandName) static constexpr CommandId commandName{ compiletimeStringHash(#commandName) }
static constexpr CommandId UNDEFINED{ 0 };

COMMAND_ID(CS_HANDSHAKE_REQUEST);
COMMAND_ID(CS_HANDSHAKE_SOLUTION);
COMMAND_ID(CS_NEW_USER);
COMMAND_ID(CS_OLD_USER);
COMMAND_ID(CS_INTERACTIVE_CHALLENGE_REPLY);

COMMAND_ID(SC_HANDSHAKE_REPLY);
COMMAND_ID(SC_HANDSHAKE_RETRY);
COMMAND_ID(SC_HANDSHAKE_SUCCESSFULL);
COMMAND_ID(SC_NEW_USER_CREATED);
COMMAND_ID(SC_AUTHORIZED);

COMMAND_ID(CC_ADD_CONTACT_REQUEST);
COMMAND_ID(CC_ADD_CONTACT_REQUEST_COMPLETED);
COMMAND_ID(CC_ADD_CONTACT_ACCEPT);
COMMAND_ID(CC_MESSAGE);

} // namespace command



#define COMMAND(commandName) class WHISPER_LIBCOMMON commandName { \
    Q_GADGET \
public: \
    static const CommandId id_{ command::commandName }; \
    operator SerializedCommand() { \
        SerializedCommand result{ command::commandName, {} }; \
        QDataStream stream(&result.data_, QIODevice::WriteOnly); \
        stream.setVersion(QDataStream::Qt_5_15); \
        const auto propertyCount = staticMetaObject.propertyCount(); \
        for (int i = 0; i < propertyCount; ++i) { \
            const auto& property = staticMetaObject.property(i); \
            stream << property.readOnGadget(this); \
        } \
        return result; \
    } \



#define FIELD(memberType, memberName) Q_PROPERTY(memberType memberName READ get_##memberName WRITE set_##memberName) \
public: \
    memberType memberName; \
    memberType get_##memberName() const { return memberName; } \
    void set_##memberName(memberType v) { memberName = v; } \



COMMAND(CS_HANDSHAKE_REQUEST)
    FIELD(QByteArray, deviceCertificate)
    FIELD(quint16, versionMajor)
    FIELD(quint16, versionMinor)
    FIELD(quint64, versionBuild)
};
COMMAND(CS_HANDSHAKE_SOLUTION)
    FIELD(QByteArray, handshakeSolution)
};
COMMAND(CS_NEW_USER)
    FIELD(QByteArray, userCertificate)
    FIELD(QByteArray, password)
};
COMMAND(CS_OLD_USER)
    FIELD(quint64, userId)
    FIELD(QString, password)
};
COMMAND(CS_INTERACTIVE_CHALLENGE_REPLY)
    FIELD(QString, challengeReply)
};
COMMAND(SC_HANDSHAKE_REPLY)
    FIELD(QString, handshakeReply)
};
COMMAND(SC_HANDSHAKE_RETRY)
};
COMMAND(SC_HANDSHAKE_SUCCESSFULL)
};
COMMAND(SC_NEW_USER_CREATED)
    FIELD(quint64, userId)
};
COMMAND(SC_AUTHORIZED)
};
COMMAND(CC_ADD_CONTACT_REQUEST)
    FIELD(quint64, userId)
};
COMMAND(CC_ADD_CONTACT_REQUEST_COMPLETED)
    FIELD(quint64, userId)
};
COMMAND(CC_ADD_CONTACT_ACCEPT)
    FIELD(quint64, userId)
};
COMMAND(CC_MESSAGE)
    FIELD(quint64, userId)
    FIELD(QByteArray, encryptedMessage)
};


} // namespace common
} // namespace whisper

#endif // PACKET_H
