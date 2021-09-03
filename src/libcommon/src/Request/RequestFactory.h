#pragma once

#include "IRequestFactory.h"

namespace whisper::common::request {

class RequestFactory final: public IRequestFactory {
    Q_DISABLE_COPY(RequestFactory)
public:
    virtual ~RequestFactory() = default;

    RequestBasePtr create(RequestType::Type type) const override;
};

} // namespace whisper::common::request
