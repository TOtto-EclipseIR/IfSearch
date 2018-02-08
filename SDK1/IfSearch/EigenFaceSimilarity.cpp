#include "EigenFaceSimilarity.h"

#include <QtCore/QList>

#include "EigenFace.h"
#include "EigenFaceSimilarityEntry.h"
#include "EigenFaceSearchSettings.h"
#include "InfoMacros.h"

EigenFaceSimilarity::EigenFaceSimilarity(EigenFaceSearchSettings * settings)
    : _settings(settings)
{
    NULLPTR(settings);
    Return::add(EigenFace::ReturnNoParameters, "INDI EigenFace No Parameters", Error);
    Return::add(EigenFace::ReturnNoData, "INDI EigenFace No Data", Error);
}

void EigenFaceSimilarity::clear(void)
{
    key_faceId_map.clear();
    key_tpl_map.clear();
    key_image_map.clear();
}

int EigenFaceSimilarity::size(void) const
{
    return key_tpl_map.size();
}

QImage EigenFaceSimilarity::image(const int faceKey) const
{
    return key_image_map.value(faceKey);
}

int EigenFaceSimilarity::insert(const EigenFaceTemplate & tpl,
                                const QString faceId,
                                const QImage & faceImage,
                                int faceKey)
{
    if ( ! faceKey)
    {
        faceKey = key_tpl_map.size();
        do ++faceKey; while (key_tpl_map.contains(faceKey));
    }

    if ( ! faceId.isEmpty())
        key_faceId_map.insertUnique(faceKey, faceId);
    key_tpl_map.insert(faceKey, tpl);
    key_image_map.insert(faceKey, faceImage);
    return faceKey;
}

int EigenFaceSimilarity::remove(const int faceKey)
{
    return key_faceId_map.remove(faceKey)
            + key_tpl_map.remove(faceKey)
            + key_image_map.remove(faceKey);
}

Return EigenFaceSimilarity::process(
        EigenFaceSimilarityResultList * results,
        const int flags)
{
    (void)flags;

    if ( ! _settings)
        return Return(EigenFace::ReturnNoParameters);
    if ( ! results)
        return Return(EigenFace::ReturnNoData);
    NULLPTR(results);

    results->clear();
    if (size() < 2)
        return Return::False;

    QList<int> faceKeys(key_tpl_map.keys());
    QMultiMap<qreal, EigenFaceSimilarityResult> mileageMMap;
    const int n = faceKeys.size();
    qreal maxDistance = _settings->fMaxDistance();

    for (int x1 = 0; x1 < n-1; ++x1)
        for (int x2 = x1+1; x2 < n; ++x2)
        {
            int faceKey1 = faceKeys.at(x1);
            int faceKey2 = faceKeys.at(x2);
            EigenFaceTemplate tpl1(key_tpl_map.value(faceKey1));
            EigenFaceTemplate tpl2(key_tpl_map.value(faceKey2));
            tpl1.setFaceKey(faceKey1);
            tpl2.setFaceKey(faceKey2);
            tpl1.setFaceId(key_faceId_map.at(faceKey1));
            tpl2.setFaceId(key_faceId_map.at(faceKey2));

            qreal distance = tpl1.distance(tpl2);
            if (distance < maxDistance)
            {
                EigenFaceSimilarityEntry entry(distance, tpl1, tpl2);
                EigenFaceSimilarityResult result(entry);
                mileageMMap.insertMulti(distance, result);
                DETAIL("FK%1~FK%2 dist=%3 inserted",
                       faceKey1, faceKey2, distance);
            }
            else
            {
                DETAIL("FK%1~FK%2 dist=%3 ignored",
                       faceKey1, faceKey2, distance);
            }
        }

    int rank = 0;
    int minConfidence = _settings->getMinConfidence();
    int maxResults = _settings->maxResults(n);
    foreach (EigenFaceSimilarityResult result, mileageMMap)
    {
        if (maxResults && results->size() >= maxResults)
        {
            DETAIL("maxResults=%1 for getMaxResults=%2",
                   maxResults, _settings->getMaxResults());
            break;
        }
        result.calculate();
        int conf = result.getConfidence();
        if (conf >= minConfidence)
        {
            result.setTier(_settings->tierForConfidence(conf));
            result.setRank(++rank);
            results->append(result);
        }
    }

    return Return::True;
}
