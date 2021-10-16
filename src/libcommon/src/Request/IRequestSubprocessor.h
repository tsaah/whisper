#pragma once

#include "RequestBase.h"

namespace whisper::common::request {

class IRequestSubprocessor: public QObject {
    Q_OBJECT
public:
    IRequestSubprocessor(QObject* parent = nullptr): QObject(parent) {}
    virtual ~IRequestSubprocessor() = default;

    virtual void processRequest(const RequestBasePtr& request) = 0;

protected:
    template<class T>
    static inline QSharedPointer<T> makeResponseForRequest(const RequestBasePtr& request) {
        return new T;
    }

signals:
    // void finished();
};

using RequestSubprocessorPtr = QSharedPointer<IRequestSubprocessor>;

} // namespace whisper::common::request
