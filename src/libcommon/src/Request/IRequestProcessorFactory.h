#pragma once

#include "IRequestProcessor.h"

namespace whisper::common::request {

class IRequestProcessorFactory {
public:
    virtual ~IRequestProcessorFactory() = default;

    virtual RequestProcessorPtr create() = 0;
};

using RequestProcessorFactoryPtr = QSharedPointer<IRequestProcessorFactory>;

} // namespace whisper::common::request
