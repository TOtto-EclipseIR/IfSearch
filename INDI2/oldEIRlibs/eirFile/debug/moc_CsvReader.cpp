/****************************************************************************
** Meta object code from reading C++ file 'CsvReader.h'
**
** Created: Tue Dec 24 16:46:15 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../CsvReader.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CsvReader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CsvReader[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,
      22,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      30,   10,   10,   10, 0x0a,
      38,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CsvReader[] = {
    "CsvReader\0\0lineRead()\0atEnd()\0start()\0"
    "lineFinished()\0"
};

const QMetaObject CsvReader::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CsvReader,
      qt_meta_data_CsvReader, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CsvReader::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CsvReader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CsvReader::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CsvReader))
        return static_cast<void*>(const_cast< CsvReader*>(this));
    return QObject::qt_metacast(_clname);
}

int CsvReader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: lineRead(); break;
        case 1: atEnd(); break;
        case 2: start(); break;
        case 3: lineFinished(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void CsvReader::lineRead()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CsvReader::atEnd()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE