#pragma once

#include <QtGlobal>

#if defined(WHISPER_LIBCOMMON_EXPORT)
    #define WHISPER_LIBCOMMON Q_DECL_EXPORT
#else
    #define WHISPER_LIBCOMMON Q_DECL_IMPORT
#endif
