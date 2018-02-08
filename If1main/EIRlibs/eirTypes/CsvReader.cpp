#include "CsvReader.h"

#include "../eirCore/eirCore.h"

#include "CsvReaderMachine.h"

const QVariant CsvReader::endLine(QChar('\n'));

CsvReader::CsvReader(QIODevice * ioDevice,
                     QObject *parent)
    : QObject(parent)
    , _stream(ioDevice)
    , _machine(new CsvReaderMachine(this))
    , fieldDelimiter_c(',')
    , beginDelimiter_c('"')
    , endDelimiter_c('"')
    , readTakes_b(true)
    , read_index(0)
{
    qDebug() << "CsvReader c'tor";
    OBJPOINTER(QIODevice, ioDevice);
    OBJPOINTER(CsvReaderMachine, _machine);
    setObjectName("CsvReader");
    if ( ! ioDevice->isReadable())
        ioDevice->open(QIODevice::ReadOnly);
    _machine->initialize();
    connect(_machine, SIGNAL(finished()),
            this, SLOT(lineFinished()));
}

bool CsvReader::isFieldDelimiter(const QChar c) const
{
    return fieldDelimiter_c == c;
}

bool CsvReader::isBeginDelimiter(const QChar c) const
{
    return beginDelimiter_c == c;
}

bool CsvReader::isEndDelimiter(const QChar c) const
{
    return endDelimiter_c == c;
}

QChar CsvReader::endDelimiter(void) const
{
    return endDelimiter_c;
}

void CsvReader::start(void)
{
    //qDebug() << "CsvReader::start()";
    OBJPOINTER(CsvReaderMachine, _machine);
    QString s(_stream.readLine());
    qDebug() << s;
    if (s.isNull())
        emit atEnd();
    else
        _machine->startWith(s);
}

void CsvReader::lineFinished(void)
{
    //qDebug() << "CsvReader::lineFinished()";
    OBJPOINTER(CsvReaderMachine, _machine);

    var_list << _machine->fields() << endLine;
    //qDebug() << "emit LineRead()" << _machine->fields();
    emit lineRead();

    QString s(_stream.readLine());
    if (s.isNull())
        emit atEnd();
    else
        _machine->startWith(s);
}

int CsvReader::size(void) const
{
    return var_list.size();
}

bool CsvReader::canReadLine(void) const
{
    return firstLineSize() >= 0;
}

int CsvReader::firstLineSize(void) const
{
    return var_list.indexOf(endLine, read_index);
}

QVariantList CsvReader::readLine(void)
{
    //qDebug() << "CsvReader::readLine()";
    int x = firstLineSize();
    if (x < 0) return QVariantList();
    //qDebug() << var_list.size() << read_index << x;
    QVariantList result(var_list.mid(read_index, x - read_index));
    if (readTakes_b)
        var_list = var_list.mid(x+1);
    else
        read_index += x+1;
    //qDebug() << "result" << result;
    return result;
}

Matrix<QVariant> CsvReader::matrix(void)
{
    //qDebug() << "CsvReader::matrix()" << var_list;
    Matrix<QVariant> result;
    int c = firstLineSize();
    int r = size() / qMax(1, c);
    result.reserve(QSize(c, r));
    //qDebug() << "reserved:" << c << r;
    while (canReadLine())
    {
        QVariantList line(readLine());
        qDebug() << line;
        result.appendRow(line);
    }
    return result;
}
