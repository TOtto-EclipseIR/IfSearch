#ifndef EIGENFACEMATCHTIER_H
#define EIGENFACEMATCHTIER_H
#include <qglobal.h>
#ifdef INDIEF_LIB
# define INDIEF_EXPORT Q_DECL_EXPORT
#else
# define INDIEF_EXPORT Q_DECL_IMPORT
#endif

#include "../../INDI2/oldEIRlibs/eirBase/Enumeration.h"

#include <QtCore/QList>
#include <QtGui/QColor>
class QChar;

#define EIGENFACESEARCHTIER_ENUM(NV) \
    NV(Unknown, = 0)  \
    NV(NoMatch,)   \
    NV(Weak,)   \
    NV(Possible,)   \
    NV(Strong,)   \
    NV(Best,)   \
    NV(size,) \

class EigenFaceSearchTier : public Enumeration
{
public:
    DECLARE_ENUMERATION(EigenFaceSearchTier, EIGENFACESEARCHTIER_ENUM)

public:
    QChar indicator(void) const;
    QColor color(void) const;
    static QColor color(const EigenFaceSearchTier tier);
    static void setColor(const EigenFaceSearchTier tier,
                    const QColor & color);

private:
    static QList<QColor> color_list;
};

#endif // EIGENFACEMATCHTIER_H
