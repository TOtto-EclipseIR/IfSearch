/****************************************************************************
** Meta object code from reading C++ file 'FileRollover.h'
**
** Created: Tue Dec 24 16:46:18 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../FileRollover.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FileRollover.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FileRollover[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      36,   31,   13,   13, 0x0a,
      75,   70,   63,   13, 0x0a,
      94,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FileRollover[] = {
    "FileRollover\0\0rolloverNeeded()\0mode\0"
    "start(QIODevice::OpenMode)\0qint64\0"
    "data\0write(const char*)\0close()\0"
};

const QMetaObject FileRollover::staticMetaObject = {
    { &QQFile::staticMetaObject, qt_meta_stringdata_FileRollover,
      qt_meta_data_FileRollover, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FileRollover::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FileRollover::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FileRollover::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FileRollover))
        return static_cast<void*>(const_cast< FileRollover*>(this));
    return QQFile::qt_metacast(_clname);
}

int FileRollover::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QQFile::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: rolloverNeeded(); break;
        case 1: start((*reinterpret_cast< QIODevice::OpenMode(*)>(_a[1]))); break;
        case 2: { qint64 _r = write((*reinterpret_cast< const char*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< qint64*>(_a[0]) = _r; }  break;
        case 3: close(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void FileRollover::rolloverNeeded()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
