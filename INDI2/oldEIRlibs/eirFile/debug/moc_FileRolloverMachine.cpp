/****************************************************************************
** Meta object code from reading C++ file 'FileRolloverMachine.h'
**
** Created: Tue Dec 24 16:46:16 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../FileRolloverMachine.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FileRolloverMachine.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FileRolloverMachine[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      33,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x05,
      32,   20,   20,   20, 0x05,
      42,   20,   20,   20, 0x05,
      51,   20,   20,   20, 0x05,
      64,   20,   20,   20, 0x05,
      82,   20,   20,   20, 0x05,

 // slots: signature, parameters, type, tag, flags
      99,   20,   20,   20, 0x0a,
     114,   20,   20,   20, 0x0a,
     132,   20,   20,   20, 0x0a,
     145,   20,   20,   20, 0x0a,
     160,   20,   20,   20, 0x0a,
     176,   20,   20,   20, 0x0a,
     188,   20,   20,   20, 0x0a,
     202,   20,   20,   20, 0x0a,
     215,   20,   20,   20, 0x0a,
     230,   20,   20,   20, 0x0a,
     247,   20,   20,   20, 0x0a,
     263,   20,   20,   20, 0x0a,
     280,   20,   20,   20, 0x0a,
     295,   20,   20,   20, 0x0a,
     313,   20,   20,   20, 0x0a,
     326,   20,   20,   20, 0x0a,
     341,   20,   20,   20, 0x0a,
     357,   20,   20,   20, 0x0a,
     369,   20,   20,   20, 0x0a,
     383,   20,   20,   20, 0x0a,
     396,   20,   20,   20, 0x0a,
     411,   20,   20,   20, 0x0a,
     428,   20,   20,   20, 0x0a,
     451,  444,   20,   20, 0x0a,
     486,  481,   20,   20, 0x0a,
     512,   20,   20,   20, 0x0a,
     519,   20,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FileRolloverMachine[] = {
    "FileRolloverMachine\0\0doConfig()\0"
    "doStart()\0doStop()\0doRollover()\0"
    "archiveComplete()\0removeComplete()\0"
    "enterInitial()\0enterConfigured()\0"
    "enterFirst()\0enterRunning()\0enterRollover()\0"
    "enterStop()\0enterRename()\0enterStart()\0"
    "enterArchive()\0enterArchiving()\0"
    "enterRemoving()\0enterTerminate()\0"
    "leaveInitial()\0leaveConfigured()\0"
    "leaveFirst()\0leaveRunning()\0leaveRollover()\0"
    "leaveStop()\0leaveRename()\0leaveStart()\0"
    "leaveArchive()\0leaveArchiving()\0"
    "leaveRemoving()\0config\0"
    "configure(FileRolloverConfig)\0mode\0"
    "open(QIODevice::OpenMode)\0stop()\0"
    "terminate()\0"
};

const QMetaObject FileRolloverMachine::staticMetaObject = {
    { &QStateMachine::staticMetaObject, qt_meta_stringdata_FileRolloverMachine,
      qt_meta_data_FileRolloverMachine, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FileRolloverMachine::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FileRolloverMachine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FileRolloverMachine::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FileRolloverMachine))
        return static_cast<void*>(const_cast< FileRolloverMachine*>(this));
    return QStateMachine::qt_metacast(_clname);
}

int FileRolloverMachine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QStateMachine::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: doConfig(); break;
        case 1: doStart(); break;
        case 2: doStop(); break;
        case 3: doRollover(); break;
        case 4: archiveComplete(); break;
        case 5: removeComplete(); break;
        case 6: enterInitial(); break;
        case 7: enterConfigured(); break;
        case 8: enterFirst(); break;
        case 9: enterRunning(); break;
        case 10: enterRollover(); break;
        case 11: enterStop(); break;
        case 12: enterRename(); break;
        case 13: enterStart(); break;
        case 14: enterArchive(); break;
        case 15: enterArchiving(); break;
        case 16: enterRemoving(); break;
        case 17: enterTerminate(); break;
        case 18: leaveInitial(); break;
        case 19: leaveConfigured(); break;
        case 20: leaveFirst(); break;
        case 21: leaveRunning(); break;
        case 22: leaveRollover(); break;
        case 23: leaveStop(); break;
        case 24: leaveRename(); break;
        case 25: leaveStart(); break;
        case 26: leaveArchive(); break;
        case 27: leaveArchiving(); break;
        case 28: leaveRemoving(); break;
        case 29: configure((*reinterpret_cast< FileRolloverConfig(*)>(_a[1]))); break;
        case 30: open((*reinterpret_cast< QIODevice::OpenMode(*)>(_a[1]))); break;
        case 31: stop(); break;
        case 32: terminate(); break;
        default: ;
        }
        _id -= 33;
    }
    return _id;
}

// SIGNAL 0
void FileRolloverMachine::doConfig()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void FileRolloverMachine::doStart()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void FileRolloverMachine::doStop()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void FileRolloverMachine::doRollover()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void FileRolloverMachine::archiveComplete()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void FileRolloverMachine::removeComplete()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}
QT_END_MOC_NAMESPACE
