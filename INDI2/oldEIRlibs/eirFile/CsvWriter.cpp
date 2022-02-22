#include "CsvWriter.h"

#include <QIODevice>

CsvWriter::CsvWriter(QIODevice * ioDevice)
    : _ioDevice(ioDevice)
    , _textStream(_ioDevice)
    , csvRows(0)
{
}

CsvWriter::CsvWriter(const QString & fileName)
    : _file(fileName)
    , _ioDevice(&_file)
    , _textStream(_ioDevice)
    , csvRows(0)
{
    _file.open(QIODevice::WriteOnly);
}

bool CsvWriter::isOpen(void) const
{
    return _ioDevice->isWritable();
}

void CsvWriter::add(VariableGroup * group,
                    VariableIdList ids)
{
    if (ids.isEmpty())
        ids = group->ids();
    foreach (VariableId id, ids)
        _list.append(QPair<VariableId, VariableGroup *>(id, group));
    csvRows = qMax(csvRows, group->csvHeadingRowCount());
}

void CsvWriter::header(void)
{
    for (int row = 0; row < csvRows; ++row)
    {
        foreach(VariablePair pair, _list)
        {
            VariableId id = pair.first;
            VariableGroup * g = pair.second;
            QString s(",");
            if (g)
                s.prepend(g->at(id).csvEntryHeading(row));
            _textStream << s;
        }
        _textStream << Qt::endl;
    }
}

void CsvWriter::writeLine(void)
{
    foreach(VariablePair pair, _list)
    {
        VariableId id = pair.first;
        VariableGroup * g = pair.second;
        QString s(",");
        if (g)
            s.prepend(g->at(id).csvEntryString());
        _textStream << s;
    }
    _textStream << Qt::endl;
}

void CsvWriter::flush(void)
{
    _textStream.flush();
}

void CsvWriter::close(void)
{
    _ioDevice->close();
}

QString CsvWriter::errorString(void) const
{
    return _ioDevice->errorString();
}
