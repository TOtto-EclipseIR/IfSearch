#ifndef GALLERYSCENE_H
#define GALLERYSCENE_H

#include <QGraphicsScene>

class Gallery;

class GalleryScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum
    {
        PixmapZ = 10,
        YinYangZ = 20,
        SelectZ,
        ScoreBorderZ = 30,
        ScoreZ,
        TitleBackgroundZ = 40,
        TitleZ,
    };

public:
    explicit GalleryScene(Gallery * parent);

signals:

public slots:

private:
    Gallery * _gallery;
};

#endif // GALLERYSCENE_H
