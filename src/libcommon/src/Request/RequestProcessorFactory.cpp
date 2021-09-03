#include "RequestProcessor.h"
#include "RequestProcessorFactory.h"

namespace whisper::common::request {

RequestProcessorPtr RequestProcessorFactory::create() {
    return QSharedPointer<RequestProcessor>::create().dynamicCast<IRequestProcessor>();
}

} // namespace whisper::common::request
