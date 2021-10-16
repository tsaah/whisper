#pragma once

#include <QtGlobal>

namespace whisper {

namespace internal {

template <size_t N, size_t I = 0>
struct hash_calc {
    static constexpr size_t apply(const char (&s)[N]) {
        return (hash_calc<N, I + 1>::apply(s) ^ s[I]) * 16777619u;
    };
};

template <size_t N>
struct hash_calc<N, N> {
    static constexpr size_t apply(const char (&s)[N]) {
        Q_UNUSED(s);
        return 2166136261u;
    };
};

} // namespace internal

template <size_t N>
constexpr size_t compiletimeStringHash(const char (&s)[N]) {
    return internal::hash_calc<N>::apply(s);
}

} // namespace whisper
