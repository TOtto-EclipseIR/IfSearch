#ifndef MESSAGEIMAGEGENERATOR_H
#define MESSAGEIMAGEGENERATOR_H
#include "eirImage.h"

#include <QtGui/QColor>
#include <QtGui/QIcon>

#include "../eirBase/Severity.h"

#include "MessageImageConfiguration.h"

class Result;
class ImageEntity;

class EIRIMAGESHARED_EXPORT MessageImageGenerator
{
public:
    MessageImageGenerator(void);
    void setConfig(const MessageImageConfiguration & config);
    ImageEntity generate(const Result & rs,
                         const int width=0);
    ImageEntity generate(const Severity sev,
                         const QString & title,
                         const QString & text,
                         const quint64 code=0,
                         int width=0);
    ImageEntity generate(const int max,
                         const int min,
                         const int value,
                         const Severity newSev=Severity());
    ImageEntity generate(const int value,
                         const Severity newSev=Severity());

private:
    void calculateRects(const int width);
    void calculatePainters(const int width);
    void updateSeverity(QPainter * painter);
    void updateProgress(QPainter * painter);
    ImageEntity entity(void);

private:
    int width_i;
    QSize frame_size;
    QRect icon_rect;
    QRect field_rect;
    QRect severity_rect;
    QRect code_rect;
    QRect title_rect;
    QRect text_rect;
    QRect progress_rect;

    QBrush frame_brush;
    QBrush code_brush;
    QBrush title_brush;
    QBrush text_brush;
    QBrush progress_brush;
    QFont severity_font;
    QFont code_font;
    QFont title_font;
    QFont text_font;
    QPen severity_pen;
    QPen code_pen;
    QPen title_pen;
    QPen text_pen;

    QImage current_image;
    Severity current_sev;
    QString currentTitle_s;
    QString currentText_s;
    quint64 currentCode_u64;
    int currentMax_i;
    int currentMin_i;
    int currentValue_i;

    MessageImageConfiguration _config;
};

#endif // MESSAGEIMAGEGENERATOR_H
