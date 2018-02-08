#ifndef CSVREADER_H
#define CSVREADER_H
#include "eirVariable.h"

#include <QtCore/QObject>
#include <QtCore/QTextStream>
#include <QtCore/QVariant>
#include <QtCore/QVariantList>

#include "../eirCollection/Matrix.h"
class VariableGroup;
class CsvReaderMachine;

class EIRVARIABLESHARED_EXPORT CsvReader : public QObject
{
    Q_OBJECT
public:
    explicit CsvReader(QIODevice * ioDevice,
                       QObject * parent=0);
    int size(void) const;
    bool canReadLine(void) const;
    int firstLineSize(void) const;
    QVariantList readLine(void);
    Matrix<QVariant> matrix(void);
    bool isFieldDelimiter(const QChar c) const;
    bool isBeginDelimiter(const QChar c) const;
    bool isEndDelimiter(const QChar c) const;
    QChar endDelimiter(void) const;

signals:
    void lineRead(void);
    void atEnd(void);

public slots:
    void start(void);

private slots:
    void lineFinished(void);

public:
    static const QVariant endLine;

private:
    void processLine(const QString & line);

private:
    QTextStream _stream;
    CsvReaderMachine * _machine;
    QChar fieldDelimiter_c;
    QChar beginDelimiter_c;
    QChar endDelimiter_c;
    QVariantList var_list;
    bool readTakes_b;
    int read_index;
};

#endif // CSVREADER_H
