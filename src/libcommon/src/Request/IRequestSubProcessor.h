#pragma once

#include "RequestBase.h"

namespace whisper::common::request {

class IRequestSubProcessor: public QObject {
    Q_OBJECT
public:
    IRequestSubProcessor(QObject* parent = nullptr): QObject(parent) {}
    virtual ~IRequestSubProcessor() = default;

    virtual void processRequest(const RequestBasePtr& request) = 0;

protected:
    template<class T>
    static inline QSharedPointer<T> makeResponseForRequest(const RequestBasePtr& request) {
        return new T;
    }

signals:
    // void finished();
};

using RequestSubProcessorPtr = QSharedPointer<IRequestSubProcessor>;

} // namespace whisper::common::request
