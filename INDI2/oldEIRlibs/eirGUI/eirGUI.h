#ifndef EIRGUI_H
#define EIRGUI_H
#ifndef __MODULE__
# define __MODULE__ "eirGUI"
#endif
#include "eirGUI_global.h"

class QImage;
class QSize;

/*! @class eirGUI
  * @brief The eirGUI library provides customized user interface elements.
  */
class EIRGUISHARED_EXPORT eirGUI
{
public:
    eirGUI(void);
    static QImage eirLogo(const QSize size);
    static QImage fromSvg(const QString & svgFilename,
                          const QSize size);
};

#endif // EIRGUI_H
