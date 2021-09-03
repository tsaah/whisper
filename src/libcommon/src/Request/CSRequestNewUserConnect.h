#pragma once

#include "Crypto/PublicKey.h"
#include "RequestBase.h"

BEGIN_CS_REQUEST_DEFINITION(NewUserConnect)

crypto::PublicKey clientKey;
crypto::PublicKey deviceKey;

REQUEST_CS_SERIALIZE_IMPLEMENTATION(NewUserConnect)
{
    if (!clientKey.isValid() || !deviceKey.isValid()) {
        return false;
    }

    stream << clientKey.toByteArray();
    stream << deviceKey.toByteArray();

    return true;
}

REQUEST_CS_DESERIALIZE_IMPLEMENTATION(NewUserConnect)
{
    {
        QByteArray ba;
        stream >> ba;
        if (!clientKey.fromByteArray(ba)) {
            return false;
        }
    }
    {
        QByteArray ba;
        stream >> ba;
        if (!deviceKey.fromByteArray(ba)) {
            return false;
        }
    }
    return true;
}

END_CS_REQUEST_DEFINITION(NewUserConnect)
