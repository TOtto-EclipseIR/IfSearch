/****************************************************************************
** Meta object code from reading C++ file 'LogDevice.h'
**
** Created: Tue Dec 24 16:47:04 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../LogDevice.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LogDevice.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LogDevice[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   11,   10,   10, 0x0a,
      29,   10,   10,   10, 0x0a,
      37,   10,   10,   10, 0x0a,
      46,   10,   10,   10, 0x0a,
      57,   10,   10,   10, 0x0a,
      75,   65,   10,   10, 0x0a,
     104,   90,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_LogDevice[] = {
    "LogDevice\0\0li\0write(LogItem)\0start()\0"
    "finish()\0rollover()\0flush()\0beforeEms\0"
    "remove(qint64)\0beforeEms,sev\0"
    "thin(qint64,int)\0"
};

const QMetaObject LogDevice::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_LogDevice,
      qt_meta_data_LogDevice, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LogDevice::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LogDevice::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LogDevice::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LogDevice))
        return static_cast<void*>(const_cast< LogDevice*>(this));
    return QObject::qt_metacast(_clname);
}

int LogDevice::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: write((*reinterpret_cast< LogItem(*)>(_a[1]))); break;
        case 1: start(); break;
        case 2: finish(); break;
        case 3: rollover(); break;
        case 4: flush(); break;
        case 5: remove((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 6: thin((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
