#ifndef QQSOUND_H
#define QQSOUND_H
#include "eirTypes.h"

#include <QtGui/QSound>

class EIRTYPESSHARED_EXPORT QQSound : public QSound
{
    Q_OBJECT
public:
    explicit QQSound(const QString & filename,
                     QObject * parent=0);

    static bool play(const QString &filename);

signals:

public slots:

};

#endif // QQSOUND_H
