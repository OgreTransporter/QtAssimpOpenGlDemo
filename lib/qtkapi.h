#ifndef QTKAPI_H
#define QTKAPI_H
#include <QtCore/QtGlobal>

#ifdef QTK_SHARED
#  if defined(QTK_EXPORT)
#    define QTKAPI Q_DECL_EXPORT
#  else
#    define QTKAPI Q_DECL_IMPORT
#  endif
#else
#  define QTKAPI
#endif

#endif // QTKAPI_H
