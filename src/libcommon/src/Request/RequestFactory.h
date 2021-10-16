#pragma once

#include "IRequestFactory.h"

namespace whisper::common::request {

class RequestFactory final: public IRequestFactory {
//    Q_DISABLE_COPY(RequestFactory)
public:
    RequestBasePtr create(RequestType::Type type) const override;
};

using RequestFactoryPtr = QSharedPointer<RequestFactory>;

} // namespace whisper::common::request
