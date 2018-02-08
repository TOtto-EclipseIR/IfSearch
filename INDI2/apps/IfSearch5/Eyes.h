/*! \file Eyes.h Declarations for Eyes class
  *
  */

#ifndef EYES_H
#define EYES_H

#include "Eyes.hh"

#include <QtCore/QLine>
#include <QtCore/QPoint>

/*! \class  Eyes
  * \brief  integer-based eye line
  * \see    TEyes - template class declaration for details
  */
typedef TEyes<QLine, QPoint, int> Eyes;
Q_DECLARE_TYPEINFO(Eyes, Q_MOVABLE_TYPE);

/*! \class  EyesF
  * \brief  float-based eye line
  * \see    TEyes - template class declaration for details
  */
typedef TEyes<QLineF, QPointF, qreal> EyesF;
Q_DECLARE_TYPEINFO(EyesF, Q_MOVABLE_TYPE);

#endif // EYES_H
