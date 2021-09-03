#include "SessionHandler.h"

namespace whisper::common {

SessionHandler::SessionHandler(const request::RequestProcessorFactoryPtr& requestProcessorFactory, const request::RequestProtocolConverterPtr& requestProtocolConverter)
    : QObject(nullptr)
{

}

void SessionHandler::processData(const QByteArray& data) {

}

} // namespace whisper::common
