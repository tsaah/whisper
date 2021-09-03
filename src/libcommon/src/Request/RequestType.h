#pragma once

#include <QMetaEnum>

namespace whisper::common::request {

class RequestType final {
    Q_GADGET
public:
    enum Type {
        Unknown,
        CSNewUserConnect, // TODO: generate random values for types
    };
    Q_ENUM(Type);

    static Type fromString(const QString& key) {
        const auto metaEnum = QMetaEnum::fromType<Type>();
        auto ok = false;
        const auto value = metaEnum.keyToValue(qPrintable(key), &ok);
        if (!ok || value == -1) {
            return Type::Unknown;
        }
        return static_cast<Type>(value);
    }

    static QString toString(Type value) {
        if (value == Unknown) {
            return {};
        }
        const auto metaEnum = QMetaEnum::fromType<Type>();
        const auto index = static_cast<int>(value);
        return metaEnum.valueToKey(index);
    }
};

} // namespace whisper::common::request
