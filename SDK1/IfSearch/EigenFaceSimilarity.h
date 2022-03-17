#ifndef EIGENFACESIMILARITY_H
#define EIGENFACESIMILARITY_H
#include <qglobal.h>

#include <QtCore/QMap>
#include <QtCore/QString>
#include <QImage>

#include "DualMap.h"
#include "EigenFaceTemplate.h"
#include "EigenFaceSimilarityResultList.h"
#include "Return.h"

class EigenFaceSearchSettings;

class INDIEF_EXPORT EigenFaceSimilarity
{
public:
    EigenFaceSimilarity(EigenFaceSearchSettings * settings);
    void clear(void);
    int size(void) const;
    QImage image(const int faceKey) const;
    int insert(const EigenFaceTemplate & tpl,
               const QString faceId,
               const QImage & faceImage=QImage(),
               int faceKey=0);
    int remove(const int faceKey);
    Return process(EigenFaceSimilarityResultList * results,
                   const int flags=0);

private:
    EigenFaceSearchSettings * _settings;
    DualMap<int, QString> key_faceId_map;
    QMap<int, EigenFaceTemplate> key_tpl_map;
    QMap<int, QImage> key_image_map;
};

#endif // EIGENFACESIMILARITY_H
