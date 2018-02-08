#ifndef EIGENFACESEARCHRESULT_H
#define EIGENFACESEARCHRESULT_H
#include <qglobal.h>
#ifdef INDIEF_LIB
# define INDIEF_EXPORT Q_DECL_EXPORT
#else
# define INDIEF_EXPORT Q_DECL_IMPORT
#endif


#include <QSharedData>
#include <QSharedDataPointer>

#include <eirBase/DataProperty.h>

#define SEARCHRESULT_DATAPROPS(TND) \
    TND(int, Rank, 0) \
    TND(int, PersonKey, 0) \
    TND(int, FaceKey, 0) \
    TND(int, SearchKey, 0) \
    TND(int, SearchVector, 0) \
    TND(int, EnrollVector, 0) \
    TND(qreal, Distance, 999.9) \
    TND(int, Confidence, 0) \


class EigenFaceSearchResultData : public QSharedData
{
    DECLARE_CHILD_DATAPROPS(SEARCHRESULT_DATAPROPS)
public:
    EigenFaceSearchResultData(void)
    {
        DEFINE_DATAPROPS_CTORS(SEARCHRESULT_DATAPROPS)
    }
};

class INDIEF_EXPORT EigenFaceSearchResult
{
    DECLARE_PARENT_DATAPROPS(SEARCHRESULT_DATAPROPS)
public:
    EigenFaceSearchResult(void);
    EigenFaceSearchResult(const EigenFaceSearchResult &);
    EigenFaceSearchResult &operator=(const EigenFaceSearchResult &);
    EigenFaceSearchResult(const int personKey,
                          const int faceKey,
                          const int searchKey,
                          const int enrollVector,
                          const int searchVector,
                          const qreal distance,
                          const int confidence);
    ~EigenFaceSearchResult();

private:
    QSharedDataPointer<EigenFaceSearchResultData> data;
};

#endif // EIGENFACESEARCHRESULT_H
