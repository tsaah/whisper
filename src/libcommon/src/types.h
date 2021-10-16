#pragma once

#include <QByteArray>

#include <cstdint>

namespace whisper {

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using f32 = float;
using f64 = double;

using Magic = u32;
using Size = i64;
using CommandId = u64;
using Version = u16;
using Flags = u32;
using RequestTypeId = u32;
using Checksum = u16;

} // namespace whisper
