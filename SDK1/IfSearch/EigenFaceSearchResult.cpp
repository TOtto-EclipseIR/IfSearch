#include "EigenFaceSearchResult.h"


EigenFaceSearchResult::
    EigenFaceSearchResult(const int personKey,
                          const int faceKey,
                          const int searchKey,
                          const int enrollVector,
                          const int searchVector,
                          const qreal distance,
                          const int confidence)
{
    PersonKey = personKey;
    FaceKey = faceKey;
    SearchKey = searchKey;
    EnrollVector = enrollVector;
    SearchVector = searchVector;
    Distance = distance;
    Confidence = confidence;
}
