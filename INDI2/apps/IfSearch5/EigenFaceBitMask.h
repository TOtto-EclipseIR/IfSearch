/** @file EigenFaceBitMask.h
*
*	Internal declarations for INDI project EigenFaceGenerator generation
*/

#pragma once

#include <QBitArray>
#include <QImage>
#include <QSize>
class QBitmap;
class QDomDocument;
class QPixmap;

#include <Return.h>

class EigenFaceData;

class EigenFaceBitMask
{
public:
    enum { StreamId=0x4546424D };

public:
    EigenFaceBitMask(void);
    EigenFaceBitMask(const QByteArray & ba);
    ~EigenFaceBitMask();
    bool isValid(void) const;
    bool isNull(void) const { return maskSize.isEmpty(); }
    void clear(void);
    QSize size(void) const { return maskSize; }
    int count(void) const { return nBits; }
    bool at(int x, int y) const { return at(maskSize.width() * y + x); }
    bool at(int i) const { return data.at(i); }
    bool at(QPoint pt) const;
    bool isValid(int r, int c) const;
    bool isValid(int x) const;
    bool isValid(QPoint pt) const;
    QImage toImage(QImage::Format Format=QImage::Format_Indexed8) const;
    QBitmap toBitmap(void) const;
    QPixmap toPixmap(void) const;
    Return readXmlFile(const QString & fileName);
    Return read(const QDomElement & de);
    Return read(const QByteArray & ba);
    Return read(QIODevice * io);
    Return write(QDomDocument * doc) const;
    Return write(QDomElement * de) const;
    Return write(QByteArray * ba) const;
    Return write(QIODevice * io) const;

private:
    QSize		maskSize;
    int			nBits;
    QBitArray	data;
}; // EigenFaceBitMask
