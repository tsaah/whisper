#include "SubprocessorCSRequestNewUserConnect.h"
#include "CSRequestNewUserConnect.h"

namespace whisper::common::request {

SubprocessorCSRequestNewUserConnect::SubprocessorCSRequestNewUserConnect()
    : IRequestSubprocessor()
{

}

void SubprocessorCSRequestNewUserConnect::processRequest(const RequestBasePtr &request) {
    auto r = request.dynamicCast<CSRequestNewUserConnect>();
    if (r.isNull()) {
        return;
    }

    // interact with user facade
    // rememner keys into connection for now
    //
}

} // namespace whisper::common::request
