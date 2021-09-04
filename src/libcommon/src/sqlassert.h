#pragma once

#include "log.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>

#define SQL_ASSERT(q, db) \
{ \
    const auto where = QString("%1").arg(__FUNCTION__); \
    const auto lastError = db.lastError(); \
    const auto what = QString("(%1) %2").arg(QString::number(lastError.type()), lastError.text()); \
    Q_ASSERT_X((q), qPrintable(where), qPrintable(what)); \
} \
