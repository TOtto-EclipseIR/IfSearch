#include "CsvReaderMachine.h"

#include <QtCore/QFinalState>

#include "../eirExe/Log.h"

#include "CsvReader.h"

const QString CsvReaderMachine::white_string("\t\n\v\f\r ");

CsvReaderMachine::CsvReaderMachine(CsvReader * parent)
    : QStateMachine(parent)
    , _reader(parent)
{
    FUNCTION();
    setObjectName("CsvReaderMachine");
}

QVariantList CsvReaderMachine::fields(void) const
{
    return field_varlist;
}

void CsvReaderMachine::initialize(void)
{
    FUNCTION();
    if (initialState())   return; // already initialized
    QState * preField_state = new QState(this);
    QState * emptyField_state = new QState(this);
    QState * wordField_state = new QState(this);
    QState * numberField_state = new QState(this);
    QState * textField_state = new QState(this);
    QState * postField_state = new QState(this);
    QFinalState * final_state = new QFinalState(this);
    OBJPOINTER(QState, preField_state);
    OBJPOINTER(QState, emptyField_state);
    OBJPOINTER(QState, wordField_state);
    OBJPOINTER(QState, numberField_state);
    OBJPOINTER(QState, textField_state);
    OBJPOINTER(QState, postField_state);
    OBJPOINTER(QFinalState, final_state);
    preField_state->setObjectName("preField_state");
    emptyField_state->setObjectName("emptyField_state");
    wordField_state->setObjectName("wordField_state");
    numberField_state->setObjectName("numberField_state");
    textField_state->setObjectName("textField_state");
    postField_state->setObjectName("postField_state");
    CONNECT(preField_state, SIGNAL(entered()), this, SLOT(enterPreField()));
    CONNECT(emptyField_state, SIGNAL(entered()), this, SLOT(enterEmptyField()));
    CONNECT(wordField_state, SIGNAL(entered()), this, SLOT(enterWordField()));
    CONNECT(numberField_state, SIGNAL(entered()), this, SLOT(enterNumberField()));
    CONNECT(textField_state, SIGNAL(entered()), this, SLOT(enterTextField()));
    CONNECT(postField_state, SIGNAL(entered()), this, SLOT(enterPostField()));
    preField_state->addTransition(this, SIGNAL(empty()), final_state);
    preField_state->addTransition(this, SIGNAL(gotDelimiter()), emptyField_state);
    preField_state->addTransition(this, SIGNAL(gotAlpha()), wordField_state);
    preField_state->addTransition(this, SIGNAL(gotNumeric()), numberField_state);
    preField_state->addTransition(this, SIGNAL(gotBegin()), textField_state);
    emptyField_state->addTransition(postField_state);
    wordField_state->addTransition(postField_state);
    numberField_state->addTransition(postField_state);
    textField_state->addTransition(postField_state);
    postField_state->addTransition(this, SIGNAL(gotDelimiter()), preField_state);
    postField_state->addTransition(this, SIGNAL(empty()), final_state);
    setInitialState(preField_state);
}

void CsvReaderMachine::clear(void)
{
    FUNCTION();
    currentLine_.clear();
    currentField_.clear();
    field_varlist.clear();
}

void CsvReaderMachine::startWith(QString line)
{
    FUNCTION(line);
    if ( ! this->initialState()) initialize();
    clear();
    currentLine_ = line;
    QStateMachine::start();
}

bool CsvReaderMachine::isWhiteSpace(const QChar c)
{
    return white_string.contains(c);
}

void CsvReaderMachine::enterPreField(void)
{
    FUNCTION();
    OBJPOINTER(CsvReader, _reader);
    while ( ! currentLine_.isEmpty())
    {
        qDebug() << currentLine_;
        const QChar first_char(currentLine_.at(0));
        if (isWhiteSpace(first_char))
        {
            currentLine_.remove(0, 1);
            continue;
        }
        else if (first_char.isDigit())
        {
            emit gotNumeric();
            return;
        }
        else if (_reader->isBeginDelimiter(first_char))
        {
            currentLine_.remove(0, 1); // the begin delimiter
            emit gotBegin(); // take to end delimiter
            return;
        }
        else
        {
            emit gotAlpha();
            return;
        }
    }
    emit empty();
}

void CsvReaderMachine::enterEmptyField(void)
{
    FUNCTION();
    field_varlist << QVariant();
}

void CsvReaderMachine::enterWordField(void)
{
    FUNCTION();
    currentField_.clear();
    while ( ! currentLine_.isEmpty())
    {
        const QChar first_char(currentLine_.at(0));
        if (isWhiteSpace(first_char))
        {
            field_varlist << currentField_;
            return;
        }
        currentField_.append(first_char);
        currentLine_.remove(0, 1);
    }
    //qDebug() << currentField_;
    field_varlist << currentField_;
}

void CsvReaderMachine::enterNumberField(void)
{
    FUNCTION();
    currentField_.clear();
    while ( ! currentLine_.isEmpty())
    {
        const QChar first_char(currentLine_.at(0));
        if ( ! first_char.isDigit())
        {
            //qDebug() << currentField_;
            field_varlist << currentField_.toUInt();
            return;
        }
        currentField_.append(first_char);
        currentLine_.remove(0, 1);
    }
    //qDebug() << currentField_;
    field_varlist << currentField_.toUInt();
}

void CsvReaderMachine::enterTextField(void)
{
    FUNCTION();
    OBJPOINTER(CsvReader, _reader);
    int pos = currentLine_.indexOf(_reader->endDelimiter());
    if (pos > 0)
    {
        field_varlist << currentLine_.mid(0, pos);
        currentLine_.remove(0, pos+1);
    }
    else
    {
        field_varlist << QString();
        currentLine_.remove(0, 1);
    }
}

void CsvReaderMachine::enterPostField(void)
{
    FUNCTION();
    OBJPOINTER(CsvReader, _reader);
    while ( ! currentLine_.isEmpty())
    {
        const QChar first_char(currentLine_.at(0));
        currentLine_.remove(0, 1);
        if (_reader->isFieldDelimiter(first_char))
        {
            //qDebug() << "emit gotDelimiter()" << first_char << currentLine_;
            emit gotDelimiter();
            return;
        }
    }
    emit empty();
}
