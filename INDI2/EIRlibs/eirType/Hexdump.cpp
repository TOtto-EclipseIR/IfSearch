/*! @file Hexdump.cpp Definitions for Hexdump class
*
*/
#include "Hexdump.h"

#include <eirBase/BaseLog.h>

#include "DataType.h"

Hexdump::Hexdump(const QVariant &var,
                 const QString &title)
{
    DataType typ(var);
    QStringList::append(typ.hexdump(var, title));
}

Hexdump::operator QStringList (void)
{
    return QStringList(*this);
}

QStringList Hexdump::from(const QByteArray & ba,
                          const QString &title)
{
    BFUNCTION(ba.size(), title);
    QStringList result;

    if ( ! title.isEmpty()) result << title;

    const quint32 * pDword = (quint32 *)ba.constData();
    const char * pChar = ba.constData();
    int offsetBytes = 0;
    int remainingBytes = ba.size();

    while (remainingBytes)
    {
        QString line;
        line += QString("+%1: ").arg(offsetBytes, 8, 16, QChar('0'));
        int x = 0;
        int cRemaining = remainingBytes;
        while (x < 4 && remainingBytes > 3)
        {
            line += QString("%1 ").arg(*pDword, 8, 16, QChar('0'));
            remainingBytes -= sizeof(quint32);
            ++pDword;
        }

        quint32 dw = *pDword;
        while (x < 4 && remainingBytes)
        {
            quint32 dw1 = dw & 0x000000FF;
            line += QString("%1").arg(dw1, 2, 16, QChar('0'));
            dw >>= 8;
            --remainingBytes;
        }

        int hexSize = 11 + (4 * 9);
        while (line.size() < hexSize)
            line += ' ';
        x = 0;
        while (x < 32 && cRemaining)
        {
            if ( ! x % 4)   line += ' ';
            char c = *pChar++;
            line += isprint(c) ? c : '.';
            ++x, --cRemaining;
        }

        result << line;
    }

    BFNRETURN(result.size());
    return result;
} // from(ba, title)
