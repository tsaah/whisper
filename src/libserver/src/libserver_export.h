#pragma once

#include <QtGlobal>

#if defined(WHISPER_LIBSERVER_EXPORT)
    #define WHISPER_LIBSERVER Q_DECL_EXPORT
#else
    #define WHISPER_LIBSERVER Q_DECL_IMPORT
#endif
