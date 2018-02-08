/****************************************************************************
** Meta object code from reading C++ file 'VariableSettingsList.h'
**
** Created: Tue Dec 24 16:46:46 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../VariableSettingsList.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VariableSettingsList.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_VariableSettingsList[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      22,   21,   21,   21, 0x05,
      32,   21,   21,   21, 0x05,
      52,   42,   21,   21, 0x05,

 // slots: signature, parameters, type, tag, flags
      80,   21,   21,   21, 0x08,
      89,   21,   21,   21, 0x0a,
      97,   21,   21,   21, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_VariableSettingsList[] = {
    "VariableSettingsList\0\0started()\0"
    "stopped()\0key,value\0triggered(QString,QVariant)\0"
    "update()\0start()\0stop()\0"
};

const QMetaObject VariableSettingsList::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_VariableSettingsList,
      qt_meta_data_VariableSettingsList, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &VariableSettingsList::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *VariableSettingsList::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *VariableSettingsList::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VariableSettingsList))
        return static_cast<void*>(const_cast< VariableSettingsList*>(this));
    return QObject::qt_metacast(_clname);
}

int VariableSettingsList::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: started(); break;
        case 1: stopped(); break;
        case 2: triggered((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2]))); break;
        case 3: update(); break;
        case 4: start(); break;
        case 5: stop(); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void VariableSettingsList::started()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void VariableSettingsList::stopped()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void VariableSettingsList::triggered(QString _t1, QVariant _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
