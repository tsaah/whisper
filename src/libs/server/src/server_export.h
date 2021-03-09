#ifndef SERVER_EXPORT_H
#define SERVER_EXPORT_H

#include <QtGlobal>

#if defined(WHISPER_SERVER_LIB_EXPORT)
    #define WHISPER_SERVER_LIB Q_DECL_EXPORT
#else
    #define WHISPER_SERVER_LIB Q_DECL_IMPORT
#endif

#endif // SERVER_EXPORT_H
