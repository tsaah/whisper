#pragma once

#include "IRequestSubprocessor.h"

namespace whisper::common::request {

class SubprocessorCSRequestNewUserConnect final: public IRequestSubprocessor {
    Q_OBJECT
public:
    SubprocessorCSRequestNewUserConnect();

    void processRequest(const RequestBasePtr& request) override;
};

using SubprocessorCSRequestNewUserConnectPtr = QSharedPointer<SubprocessorCSRequestNewUserConnect>;

} // namespace whisper::common::request
