#ifndef SERVER_EXPORT_H
#define SERVER_EXPORT_H

#include <QtGlobal>

#if defined(WHISPER_LIBSERVER_EXPORT)
    #define WHISPER_LIBSERVER Q_DECL_EXPORT
#else
    #define WHISPER_LIBSERVER Q_DECL_IMPORT
#endif

#endif // SERVER_EXPORT_H
