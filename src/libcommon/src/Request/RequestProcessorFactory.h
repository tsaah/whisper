#pragma once

#include "IRequestProcessorFactory.h"

namespace whisper::common::request {

class RequestProcessorFactory final: public IRequestProcessorFactory {
public:
    RequestProcessorPtr create() override;
};

} // namespace whisper::common::request
