#ifndef EIGENFACESEARCHRESULT_H
#define EIGENFACESEARCHRESULT_H
#include <qglobal.h>

class EigenFaceSearchResult
{
public:
    EigenFaceSearchResult(void) {;}
    EigenFaceSearchResult(const int personKey,
                          const int faceKey,
                          const int searchKey,
                          const int enrollVector,
                          const int searchVector,
                          const qreal distance,
                          const int confidence);

public:
    int Rank = 0;
    int PersonKey = 0;
    int FaceKey = 0;
    int SearchKey = 0;
    int SearchVector = 0;
    int EnrollVector = 0;
    qreal Distance = 999.9;
    int Confidence = 0;
};

#endif // EIGENFACESEARCHRESULT_H
