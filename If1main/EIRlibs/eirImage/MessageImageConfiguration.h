#ifndef MESSAGEIMAGECONFIGURATION_H
#define MESSAGEIMAGECONFIGURATION_H
#include "eirImage.h"

#include <QtCore/QRectF>
#include <QtCore/QSizeF>
#include <QtGui/QBrush>
#include <QtGui/QColor>
#include <QtGui/QFont>
#include <QtGui/QPen>

#include "../eirBase/Severity.h"
#include "../eirBase/SeverityArray.h"
#include "../eirTypes/QQRectF.h"
#include "../eirTypes/QQSizeF.h"
#include "../eirCore/VariableSet.h"
#include "../eirGUI/QQIcon.h"


#define MI_CONFIGURATION_VARIABLESET(TIVD) \
    TIVD(int, DefaultWidth, int, 640) \
    TIVD(QQRectF, FrameRect,     QRectF, QRect(QPoint(0,0), QSize(640, 480)) ) \
    TIVD(QQRectF, IconRect,      QRectF, QQRectF(QSizeF(2.8/16, 2.8/16), QPointF(2.5/16, 2.5/16))) \
    TIVD(QQRectF, FieldRect,     QRectF, QRectF(QPointF(1.0/16, 1.0/16), QPointF(4.0/16, 5.0/16))) \
    TIVD(QQRectF, SeverityRect,  QRectF, QQRectF(QSizeF(2.5/16, 1.0/16), QPointF(2.5/16, 4.4/16))) \
    TIVD(QQRectF, CodeRect,      QRectF, QRectF(QPointF(5.0/16, 4.2/16), QPointF(15.0/16, 4.8/16))) \
    TIVD(QQRectF, TitleRect,     QRectF, QRectF(QPointF(1.0/16, 6.0/16), QPointF(15.0/16, 11.0/16))) \
    TIVD(QQRectF, TextRect,      QRectF, QRectF(QPointF(5.0/16, 1.0/16), QPointF(15.0/16, 4.0/16))) \
    TIVD(QQRectF, ProgressRect,  QRectF, QRectF(QPointF(1.0/16, 5.2/16), QPointF(15.0/16, 5.8/16))) \
    TIVD(QString, FrameBrush,    QString, QString("black")) \
    TIVD(QString, CodeBrush,     QString, QString()) \
    TIVD(QString, TitleBrush,    QString, QString()) \
    TIVD(QString, TextBrush,     QString, QString("white")) \
    TIVD(QString, ProgressBrush, QString, QString("lightGray")) \
    TIVD(QString, SeverityFontFamily, QString, QString("Times")) \
    TIVD(QString, CodeFontFamily,     QString, QString("Helvetica")) \
    TIVD(QString, TitleFontFamily,    QString, QString("Helvetica")) \
    TIVD(QString, TextFontFamily,     QString, QString("Times")) \
    TIVD(int, SeverityFontPoints, int, 14) \
    TIVD(int, CodeFontPoints,     int, 8) \
    TIVD(int, TitleFontPoints,    int, 12) \
    TIVD(int, TextFontPoints,     int, 10) \
    TIVD(QString, SeverityPenColor, QString, QString("white")) \
    TIVD(QString, CodePenColor,     QString, QString("lightgray")) \
    TIVD(QString, TitlePenColor,    QString, QString("white")) \
    TIVD(QString, TextPenColor,     QString, QString("black")) \


class EIRIMAGESHARED_EXPORT MessageImageConfiguration
    : public VariableSet
{
public:
    DECLARE_VARIABLESET(MI_CONFIGURATION_VARIABLESET);
    MessageImageConfiguration(void);
    MessageImageConfiguration(const VariableSet & other);

    QSize frameSize(const int width=0) const;
    QRect fieldRect(const int width=0) const;
    QRect iconRect(const int width=0) const;
    QRect codeRect(const int width=0) const;
    QRect titleRect(const int width=0) const;
    QRect textRect(const int width=0) const;
    QRect severityRect(const int width=0) const;
    QRect progressRect(const int width=0) const;
    QBrush frameBrush(void) const;
    QBrush codeBrush(void) const;
    QBrush titleBrush(void) const;
    QBrush textBrush(void) const;
    QBrush progressBrush(void) const;
    QPen codePen(void) const;
    QPen severityPen(void) const;
    QPen textPen(void) const;
    QPen titlePen(void) const;
    QFont codeFont(void) const;
    QFont severityFont(void) const;
    QFont textFont(void) const;
    QFont titleFont(void) const;
    QBrush severityBrush(const Severity sev);
    QIcon severityIcon(const Severity sev);
    void initialize(void);

private:
    SeverityArray<QIcon> icon_sevArray;
    SeverityArray<QColor> color_sevArray;
};

#endif // MESSAGEIMAGECONFIGURATION_H
