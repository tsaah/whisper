#ifndef COMMON_EXPORT_H
#define COMMON_EXPORT_H

#include <QtGlobal>

#if defined(WHISPER_COMMON_LIB_EXPORT)
    #define WHISPER_COMMON_LIB Q_DECL_EXPORT
#else
    #define WHISPER_COMMON_LIB Q_DECL_IMPORT
#endif

#endif // COMMON_EXPORT_H
