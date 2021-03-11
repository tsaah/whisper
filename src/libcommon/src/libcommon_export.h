#ifndef COMMON_EXPORT_H
#define COMMON_EXPORT_H

#include <QtGlobal>

#if defined(WHISPER_LIBCOMMON_EXPORT)
    #define WHISPER_LIBCOMMON Q_DECL_EXPORT
#else
    #define WHISPER_LIBCOMMON Q_DECL_IMPORT
#endif

#endif // COMMON_EXPORT_H
