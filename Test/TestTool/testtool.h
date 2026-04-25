#ifndef TESTTOOL_H
#define TESTTOOL_H

#include <QObject>
#include <QJsonDocument>

#include "apihelper.h"

#if defined(TESTTOOL_SHARED)
#  if defined(TESTTOOL_LIBRARY)
#    define TESTTOOL_EXPORT Q_DECL_EXPORT
#  else
#    define TESTTOOL_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define TESTTOOL_EXPORT
#endif

namespace TestTool
{
TESTTOOL_EXPORT void init();
TESTTOOL_EXPORT const QJsonDocument& config();
TESTTOOL_EXPORT ApiHelper* helper();
};

#endif // TESTTOOL_H
