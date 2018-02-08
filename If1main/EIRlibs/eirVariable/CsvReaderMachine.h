#ifndef CSVREADERMACHINE_H
#define CSVREADERMACHINE_H

#include <QtCore/QStateMachine>
#include <QtCore/QString>
#include <QtCore/QVariantList>

class CsvReader;

class CsvReaderMachine : public QStateMachine
{
    Q_OBJECT
public:
    explicit CsvReaderMachine(CsvReader * parent);
    QVariantList fields(void) const;

signals:
    void gotDelimiter(void);
    void gotAlpha(void);
    void gotBegin(void);
    void gotNumeric(void);
    void empty(void);

public slots:
    void initialize(void);
    void clear(void);
    void startWith(QString line);

private slots:
    void enterPreField(void);
    void enterEmptyField(void);
    void enterWordField(void);
    void enterNumberField(void);
    void enterTextField(void);
    void enterPostField(void);

private:
    static bool isWhiteSpace(const QChar c);

private:
    CsvReader * _reader;
    QString currentLine_;
    QString currentField_;
    QVariantList field_varlist;
    static const QString white_string;
};

#endif // CSVREADERMACHINE_H
