#ifndef QCLOUDMUSICAPIGLOBAL_H
#define QCLOUDMUSICAPIGLOBAL_H

#include <QtCompilerDetection>

#include "definevaluehelper.h"

#if defined(BUILD_SHARED_LIBS)
#if defined(QCLOUDMUSICAPI_LIBRARY)
#  define QCLOUDMUSICAPI_EXPORT Q_DECL_EXPORT
#else
#  define QCLOUDMUSICAPI_EXPORT Q_DECL_IMPORT
#endif
#else
#  define QCLOUDMUSICAPI_EXPORT
#endif

#endif // QCLOUDMUSICAPIGLOBAL_H
