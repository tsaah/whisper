#pragma once

#include "RequestBase.h"

namespace whisper::common::request {

class IRequestFactory {
    Q_DISABLE_COPY(IRequestFactory)
public:
    virtual ~IRequestFactory() = default;

    virtual RequestBasePtr create(RequestType::Type type) const = 0;
};

using RequestFactoryPtr = QSharedPointer<IRequestFactory>;

} // namespace whisper::common::request
