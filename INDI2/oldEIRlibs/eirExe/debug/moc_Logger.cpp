/****************************************************************************
** Meta object code from reading C++ file 'Logger.h'
**
** Created: Tue Dec 24 16:47:05 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Logger.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Logger.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Logger[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,    8,    7,    7, 0x05,
      37,    8,    7,    7, 0x05,
      60,    8,    7,    7, 0x05,
      96,   82,    7,    7, 0x05,
     127,    8,    7,    7, 0x05,
     149,   82,    7,    7, 0x05,
     182,    8,    7,    7, 0x05,
     204,   82,    7,    7, 0x05,
     230,   82,    7,    7, 0x05,
     262,    8,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
     283,    7,    7,    7, 0x0a,
     291,    7,    7,    7, 0x08,
     298,    8,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Logger[] = {
    "Logger\0\0name\0rolloverNeeded(QString)\0"
    "rolloverEnter(QString)\0rolloverStop(QString)\0"
    "name,fileName\0rolloverStart(QString,QString)\0"
    "rolloverExit(QString)\0"
    "rolloverDeleted(QString,QString)\0"
    "archiveEnter(QString)\0archived(QString,QString)\0"
    "archiveDeleted(QString,QString)\0"
    "archiveExit(QString)\0flush()\0pump()\0"
    "doRollover(QString)\0"
};

const QMetaObject Logger::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Logger,
      qt_meta_data_Logger, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Logger::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Logger::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Logger::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Logger))
        return static_cast<void*>(const_cast< Logger*>(this));
    return QObject::qt_metacast(_clname);
}

int Logger::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: rolloverNeeded((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: rolloverEnter((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: rolloverStop((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: rolloverStart((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 4: rolloverExit((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: rolloverDeleted((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 6: archiveEnter((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: archived((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 8: archiveDeleted((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 9: archiveExit((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: flush(); break;
        case 11: pump(); break;
        case 12: doRollover((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void Logger::rolloverNeeded(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Logger::rolloverEnter(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Logger::rolloverStop(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Logger::rolloverStart(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Logger::rolloverExit(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Logger::rolloverDeleted(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Logger::archiveEnter(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void Logger::archived(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void Logger::archiveDeleted(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void Logger::archiveExit(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}
QT_END_MOC_NAMESPACE
