#ifndef LOG_H
#define LOG_H

#include <QDebug>
#include <QThread>
#include <QDateTime>

#define wDebug qDebug() << '.' << qPrintable(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz")) << QThread::currentThread() << __FUNCTION__
#define wInfo qInfo() << 'i' << qPrintable(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz")) << QThread::currentThread() << __FUNCTION__
#define wWarn qWarning() << 'w' << qPrintable(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz")) << QThread::currentThread() << __FUNCTION__ << __FILE__ << __LINE__
#define wError qCritical() << 'E' << qPrintable(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz")) << QThread::currentThread() << __FUNCTION__ << __FILE__ << __LINE__

#endif // LOG_H
