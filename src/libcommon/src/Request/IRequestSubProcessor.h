#pragma once

#include "RequestBase.h"

namespace whisper::common::request {

class IRequestSubProcessor {
public:
    virtual ~IRequestSubProcessor() = default;

    virtual void processRequest(const RequestBasePtr& request) = 0;

protected:
    template<class T>
    static inline QSharedPointer<T> makeResponseForRequest(const RequestBasePtr& request) {
        return new T;
    }
};

using RequestSubProcessorPtr = QSharedPointer<IRequestSubProcessor>;

} // namespace whisper::common::request
