#ifndef QTFIREBASEDB_GLOBAL_H
#define QTFIREBASEDB_GLOBAL_H

#include <QtCore/qglobal.h>

QT_BEGIN_NAMESPACE

#ifndef QT_STATIC
#    if defined(QT_BUILD_FIREBASEDB_LIB)
#      define Q_FIREBASEDB_EXPORT Q_DECL_EXPORT
#    else
#      define Q_FIREBASEDB_EXPORT Q_DECL_IMPORT
#    endif
#else
#    define Q_FIREBASEDB_EXPORT
#endif

QT_END_NAMESPACE

#endif // QTFIREBASEDB_GLOBAL_H
