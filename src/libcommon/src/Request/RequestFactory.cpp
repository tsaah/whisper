#include "RequestFactory.h"
#include "CSRequestNewUserConnect.h"

namespace whisper::common::request {

RequestBasePtr RequestFactory::create(RequestType::Type type) const {
    switch (type) {
        case RequestType::Type::CSNewUserConnect: return CSRequestNewUserConnectPtr::create();
        default: return {};
    }
}

} // namespace whisper::common::request
