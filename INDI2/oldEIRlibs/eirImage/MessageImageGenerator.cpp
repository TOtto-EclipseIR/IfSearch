#include "MessageImageGenerator.h"

#include <QtGui/QPainter>

#include "../eirExe/Log.h"
#include "../eirExe/Result.h"
#include "../eirExe/ResultCode.h"

#include "ImageEntity.h"

MessageImageGenerator::MessageImageGenerator(void)
    : width_i(0)
{
    FUNCTION();
}

void MessageImageGenerator::setConfig(
        const MessageImageConfiguration & config)
{
    FUNCTION(config.name());
    _config = config;
    width_i = 0;
    frame_brush = QBrush();
}

ImageEntity MessageImageGenerator::generate(const Result & rs,
                                            const int width)
{
    return generate(rs.severity(),
                     rs.toString(),
                     rs.description(),
                     rs.code(),
                     width);
}

ImageEntity MessageImageGenerator::generate(
        const Severity sev,
        const QString & title,
        const QString & text,
        const quint64 code,
        int width)
{
    FUNCTION(sev.name(), title, text, code);
    if ( ! width) width = _config.getDefaultWidth();
    calculateRects(width);
    calculatePainters(width);

    current_sev = sev;
    currentTitle_s = title;
    currentText_s = text;
    currentCode_u64 = code;
    currentMax_i = currentMin_i = currentValue_i = 0;

    /*--- Create new base image ---*/
    current_image = QImage(frame_size,
                           QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&current_image);
    painter.setBackground(frame_brush);

    /*--- optional code ---*/
    if (code)
    {
        QString text = "0x"+QString::number(code, 16).toUpper()
                        +" "+QString::number(code);
        EightCC ecc(code);
        if (ecc.isPrintable())
            text.prepend("["+ecc.toString()+"] ");
        painter.fillRect(code_rect, code_brush);
        painter.setPen(code_pen);
        painter.setFont(code_font);
        painter.drawText(code_rect, Qt::AlignLeft | Qt::AlignVCenter, text);
    }

    /*--- title and text ---*/
    if ( ! title.isEmpty())
    {
        painter.fillRect(title_rect, title_brush);
        painter.setPen(title_pen);
        painter.setFont(title_font);
        painter.drawText(title_rect, Qt::AlignCenter | Qt::TextWordWrap, title);
    }
    if ( ! text.isEmpty())
    {
        painter.fillRect(text_rect, text_brush);
        painter.setPen(text_pen);
        painter.setFont(text_font);
        QQRectF text_rf(text_rect);
        DUMPVAR(text_rf.toString());
        painter.drawText(text_rect, Qt::AlignTop
                                    | Qt::AlignLeft
                                    | Qt::TextWordWrap, text);
    }

    updateSeverity(&painter);
    painter.end();

    return entity();
}

ImageEntity MessageImageGenerator::generate(
        const int max,
        const int min,
        const int value,
        const Severity newSev)
{
    FUNCTION(min, max, value, newSev.name());
    currentMin_i = min;
    currentMax_i = max;
    currentValue_i = value;

    QPainter painter(&current_image);
    if (newSev.isValid())
    {
        current_sev = newSev;
        updateSeverity(&painter);
    }
    updateProgress(&painter);
    painter.end();

    return entity();
}

ImageEntity MessageImageGenerator::generate(
        const int value,
        const Severity newSev)
{
    FUNCTION(value, newSev.name());
    currentValue_i = value;

    QPainter painter(&current_image);
    if (newSev.isValid())
    {
        current_sev = newSev;
        updateSeverity(&painter);
    }
    updateProgress(&painter);
    painter.end();

    return entity();
}

void MessageImageGenerator::calculateRects(const int width)
{
    FUNCTION(width);
    if (width == width_i)   return;

    width_i         = width;
    frame_size      = _config.frameSize(width);
    field_rect      = _config.fieldRect(width);
    icon_rect       = _config.iconRect(width);
    code_rect       = _config.codeRect(width);
    title_rect      = _config.titleRect(width);
    text_rect       = _config.textRect(width);
    severity_rect   = _config.severityRect(width);
    progress_rect   = _config.progressRect(width);
}

void MessageImageGenerator::calculatePainters(const int width)
{
    FUNCTION(width);
    if (Qt::NoBrush != frame_brush.style())     return;

    frame_brush     = _config.frameBrush();
    code_brush      = _config.codeBrush();
    title_brush     = _config.titleBrush();
    text_brush      = _config.textBrush();
    progress_brush  = _config.progressBrush();
    code_pen        = _config.codePen();
    severity_pen    = _config.severityPen();
    text_pen        = _config.textPen();
    title_pen       = _config.titlePen();
    code_font       = _config.codeFont();
    severity_font   = _config.severityFont();
    text_font       = _config.textFont();
    title_font      = _config.titleFont();
}

void MessageImageGenerator::updateSeverity(QPainter * painter)
{
    FUNCTION();
    QBrush brush = _config.severityBrush(current_sev);
    QIcon ic = _config.severityIcon(current_sev);
    QPixmap pm = ic.pixmap(icon_rect.size());

    painter->fillRect(field_rect, brush);
    painter->setPen(severity_pen);
    painter->setFont(severity_font);
    painter->drawText(severity_rect,
                     Qt::AlignVCenter | Qt::AlignJustify,
                     current_sev.name());
    painter->drawPixmap(icon_rect, pm);
}

void MessageImageGenerator::updateProgress(QPainter * painter)
{
    FUNCTION();
    if ( ! currentMax_i && ! currentMin_i)  return;

    QBrush brush = _config.progressBrush();
    QRect rect = _config.progressRect();
    QBrush bar = _config.severityBrush(current_sev);
    int left, width;
    if (currentMin_i == currentMax_i)
    {
        left = (currentValue_i % 10)
                * (rect.width() / 10) + rect.left();
        width = rect.width() / 10;
    }
    else
    {
        left = rect.left();
        width = (rect.width() * currentValue_i)
                / (currentMax_i - currentMin_i);
    }

    painter->fillRect(progress_rect, brush);
    painter->fillRect(rect.left(), rect.y(), width, rect.height(), bar);
}

ImageEntity MessageImageGenerator::entity(void)
{
    FUNCTION();
    ImageEntity ie("MessageImage:"+current_sev.name());
    ie.setImage_Class(ImageClass::Message);
    ie.set("Message/Severity", current_sev.name());
    ie.set("Message/Title", currentTitle_s);
    ie.set("Message/Text", currentText_s);
    ie.set("Message/Code", currentCode_u64);
    ie.set("Message/MinValue", currentMin_i);
    ie.set("Message/MaxValue", currentMax_i);
    ie.set("Message/Value", currentValue_i);
    ie.setQImage(current_image);
    LOG(dump(ie));
    return ie;
}
