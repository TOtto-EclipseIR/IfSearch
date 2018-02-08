#ifndef KEY_H
#define KEY_H

typedef unsigned long Key_type;

#include "Key.hh"

KEYTRAITS_DECLARE(AverageFaceKeyTraits);
KEYTRAITS_DECLARE(ImageKeyTraits);
KEYTRAITS_DECLARE(FaceKeyTraits);
KEYTRAITS_DECLARE(PersonKeyTraits);

typedef Key<AverageFaceKeyTraits> AverageFaceKey;
typedef Key<ImageKeyTraits> ImageKey;
typedef Key<FaceKeyTraits> FaceKey;
typedef Key<PersonKeyTraits> PersonKey;

#endif // KEY_H
