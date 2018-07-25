#ifndef LABELINGBASE_GLOBAL_H
#define LABELINGBASE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LABELINGBASE_LIBRARY)
#  define LABELINGBASESHARED_EXPORT Q_DECL_EXPORT
#else
#  define LABELINGBASESHARED_EXPORT Q_DECL_IMPORT
#endif

typedef void (*loadProgress)(void * data, double total , double now );

#endif // LABELINGBASE_GLOBAL_H
