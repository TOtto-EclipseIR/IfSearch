#include "KeyClass.h"

#include <QtCore/QVector>

#include "KeyClassBehavior.h"

QVector<KeyClassBehavior *> KeyClass::classData_vec(KeyClass64g);

KeyClassBehavior * KeyClass::instance(const KeyClasses keyClass)
{
    KeyClassBehavior * p = classData_vec.at(keyClass);
    if (p) return p;

    switch (keyClass)
    {
    case KeyClass16z:   p = new KeyClassBehavior(16, 1, 5, 1, 0,  0);   break;
    case KeyClass16a:   p = new KeyClassBehavior(16, 1, 5, 1, 1,  3);   break;

    case KeyClass32z:   p = new KeyClassBehavior(32, 2, 7, 2, 0,  0);   break;
    case KeyClass32a:   p = new KeyClassBehavior(32, 2, 7, 2, 1,  4);   break;
    case KeyClass32b:   p = new KeyClassBehavior(32, 2, 7, 2, 2,  7);   break;
    case KeyClass32c:   p = new KeyClassBehavior(32, 2, 7, 2, 3, 10);   break;

    case KeyClass64z:   p = new KeyClassBehavior(64, 3, 8, 3, 0,  0);   break;
    case KeyClass64a:   p = new KeyClassBehavior(64, 3, 8, 3, 1,  8);   break;
    case KeyClass64b:   p = new KeyClassBehavior(64, 3, 8, 3, 2, 12);   break;
    case KeyClass64c:   p = new KeyClassBehavior(64, 3, 8, 3, 3, 16);   break;
    case KeyClass64d:   p = new KeyClassBehavior(64, 3, 8, 3, 4, 20);   break;
    case KeyClass64e:   p = new KeyClassBehavior(64, 3, 8, 3, 5, 24);   break;
    case KeyClass64f:   p = new KeyClassBehavior(64, 3, 8, 3, 6, 28);   break;
    case KeyClass64g:   p = new KeyClassBehavior(64, 3, 8, 3, 7, 32);   break;
    case Null:
        break;
    }
    classData_vec[keyClass] = p;
    return p;
}

