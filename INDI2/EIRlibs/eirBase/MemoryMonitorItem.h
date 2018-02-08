/*! @file MemoryMonitorItem.h Declarations for MemoryMonitorItem class
*
*/
#ifndef MEMORYMONITORITEM_H
#define MEMORYMONITORITEM_H
#include "eirBase.h"

#include <QSharedData>

#include "DataProperty.h"

typedef Qt::HANDLE qtHandle;
typedef void * voidPtr;

#define MEMORYMONITORITEM_DATAPROPS(TND) \
    TND(voidPtr,    Pointer,        0) \
    TND(voidPtr,    Parent,         0) \
    TND(size_t,     Bytes,          0) \
    TND(qtHandle,   Thread,         0) \
    TND(QString,    VarName,        QString()) \
    TND(QString,    TypeName,       QString()) \
    TND(QString,    AllocFunction,  QString()) \
    TND(QString,    FreeFunction,   QString()) \
    TND(QString,    FileName,       QString()) \
    TND(int,        FileLine,       0) \
    TND(qint64,     AllocEms,       0) \
    TND(qint64,     FreeEms,        0) \
    TND(int,        Flags,          0) \

class MemoryMonitorData : public QSharedData
{
    DECLARE_CHILD_DATAPROPS(MEMORYMONITORITEM_DATAPROPS)
public:
    MemoryMonitorData(void)
    {
        DEFINE_DATAPROPS_CTORS(MEMORYMONITORITEM_DATAPROPS)
    }
};

class EIRBASESHARED_EXPORT MemoryMonitorItem
{
    DECLARE_PARENT_DATAPROPS(MEMORYMONITORITEM_DATAPROPS)
    enum Flags
    {
        isQObject       = 0x00000001,
        notQObject      = 0x00000002,
        isHeap          = 0x00000004,
        isInitial       = 0x00000008,
    };

public:
    MemoryMonitorItem(void); // default c'tor
    MemoryMonitorItem(const MemoryMonitorItem & rhs);
    MemoryMonitorItem & operator = (const MemoryMonitorItem & rhs);
    ~MemoryMonitorItem();

    bool isNull(void) const;
    void setInitial(bool on=true);

private:
    QSharedDataPointer<MemoryMonitorData> data;
};

#endif // MEMORYMONITORITEM_H
