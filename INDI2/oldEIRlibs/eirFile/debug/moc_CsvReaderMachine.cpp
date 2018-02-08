/****************************************************************************
** Meta object code from reading C++ file 'CsvReaderMachine.h'
**
** Created: Tue Dec 24 16:46:14 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../CsvReaderMachine.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CsvReaderMachine.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CsvReaderMachine[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x05,
      33,   17,   17,   17, 0x05,
      44,   17,   17,   17, 0x05,
      55,   17,   17,   17, 0x05,
      68,   17,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      76,   17,   17,   17, 0x0a,
      89,   17,   17,   17, 0x0a,
     102,   97,   17,   17, 0x0a,
     121,   17,   17,   17, 0x08,
     137,   17,   17,   17, 0x08,
     155,   17,   17,   17, 0x08,
     172,   17,   17,   17, 0x08,
     191,   17,   17,   17, 0x08,
     208,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CsvReaderMachine[] = {
    "CsvReaderMachine\0\0gotDelimiter()\0"
    "gotAlpha()\0gotBegin()\0gotNumeric()\0"
    "empty()\0initialize()\0clear()\0line\0"
    "startWith(QString)\0enterPreField()\0"
    "enterEmptyField()\0enterWordField()\0"
    "enterNumberField()\0enterTextField()\0"
    "enterPostField()\0"
};

const QMetaObject CsvReaderMachine::staticMetaObject = {
    { &QStateMachine::staticMetaObject, qt_meta_stringdata_CsvReaderMachine,
      qt_meta_data_CsvReaderMachine, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CsvReaderMachine::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CsvReaderMachine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CsvReaderMachine::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CsvReaderMachine))
        return static_cast<void*>(const_cast< CsvReaderMachine*>(this));
    return QStateMachine::qt_metacast(_clname);
}

int CsvReaderMachine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QStateMachine::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: gotDelimiter(); break;
        case 1: gotAlpha(); break;
        case 2: gotBegin(); break;
        case 3: gotNumeric(); break;
        case 4: empty(); break;
        case 5: initialize(); break;
        case 6: clear(); break;
        case 7: startWith((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: enterPreField(); break;
        case 9: enterEmptyField(); break;
        case 10: enterWordField(); break;
        case 11: enterNumberField(); break;
        case 12: enterTextField(); break;
        case 13: enterPostField(); break;
        default: ;
        }
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void CsvReaderMachine::gotDelimiter()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CsvReaderMachine::gotAlpha()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void CsvReaderMachine::gotBegin()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void CsvReaderMachine::gotNumeric()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void CsvReaderMachine::empty()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}
QT_END_MOC_NAMESPACE
