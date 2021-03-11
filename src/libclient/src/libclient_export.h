#ifndef CLIENT_EXPORT_H
#define CLIENT_EXPORT_H

#include <QtGlobal>

#if defined(WHISPER_LIBCLIENT_EXPORT)
    #define WHISPER_LIBCLIENT Q_DECL_EXPORT
#else
    #define WHISPER_LIBCLIENT Q_DECL_IMPORT
#endif

#endif // CLIENT_EXPORT_H
