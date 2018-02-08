/****************************************************************************
** Meta object code from reading C++ file 'LogDeviceTxtFile.h'
**
** Created: Tue Dec 24 16:47:06 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../LogDeviceTxtFile.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LogDeviceTxtFile.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LogDeviceTxtFile[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x05,
      35,   17,   17,   17, 0x05,
      51,   17,   17,   17, 0x05,
      75,   66,   17,   17, 0x05,
      98,   17,   17,   17, 0x05,
     113,   66,   17,   17, 0x05,
     138,   17,   17,   17, 0x05,
     153,   66,   17,   17, 0x05,
     171,   66,   17,   17, 0x05,
     195,   17,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
     209,   17,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_LogDeviceTxtFile[] = {
    "LogDeviceTxtFile\0\0rolloverNeeded()\0"
    "rolloverEnter()\0rolloverStop()\0fileName\0"
    "rolloverStart(QString)\0rolloverExit()\0"
    "rolloverDeleted(QString)\0archiveEnter()\0"
    "archived(QString)\0archiveDeleted(QString)\0"
    "archiveExit()\0doRollover()\0"
};

const QMetaObject LogDeviceTxtFile::staticMetaObject = {
    { &LogDevice::staticMetaObject, qt_meta_stringdata_LogDeviceTxtFile,
      qt_meta_data_LogDeviceTxtFile, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LogDeviceTxtFile::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LogDeviceTxtFile::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LogDeviceTxtFile::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LogDeviceTxtFile))
        return static_cast<void*>(const_cast< LogDeviceTxtFile*>(this));
    return LogDevice::qt_metacast(_clname);
}

int LogDeviceTxtFile::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = LogDevice::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: rolloverNeeded(); break;
        case 1: rolloverEnter(); break;
        case 2: rolloverStop(); break;
        case 3: rolloverStart((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: rolloverExit(); break;
        case 5: rolloverDeleted((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: archiveEnter(); break;
        case 7: archived((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: archiveDeleted((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: archiveExit(); break;
        case 10: doRollover(); break;
        default: ;
        }
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void LogDeviceTxtFile::rolloverNeeded()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void LogDeviceTxtFile::rolloverEnter()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void LogDeviceTxtFile::rolloverStop()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void LogDeviceTxtFile::rolloverStart(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void LogDeviceTxtFile::rolloverExit()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void LogDeviceTxtFile::rolloverDeleted(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void LogDeviceTxtFile::archiveEnter()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void LogDeviceTxtFile::archived(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void LogDeviceTxtFile::archiveDeleted(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void LogDeviceTxtFile::archiveExit()
{
    QMetaObject::activate(this, &staticMetaObject, 9, 0);
}
QT_END_MOC_NAMESPACE
