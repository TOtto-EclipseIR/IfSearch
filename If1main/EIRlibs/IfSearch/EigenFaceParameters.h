/** @file EigenFaceParameters.h
*
*	Internal declarations for INDI project EigenFaceGenerator generation
*/

#pragma once

#include <Eyes.h>

namespace INDI
{

    // Parameters that can be set by the application
    // for different purposes (enroll v. search)
    class INDIEF_EXPORT EigenFaceParameters
    {
    private:
        EigenFaceParameters(void);

    public:
        ~EigenFaceParameters();
        static EigenFaceParameters * create(const QDir & initDir);
        static DDT::Return lastReturn(void) { return staticReturn; }
        DDT::Return init(const QDomElement & de);
        bool isValid(void) { return true; } // TODO
        //		EigenFaceVectorType templateVector(int x) const
        //		{ return (x >= 0 && x < TemplateVectors.size()) ? TemplateVectors.at(x) : EigenFaceVectorType; }
        int maxAveragesInTemplate(void) const
        { return maxAvgsInTemplate; }
        QSize normalizedSize(void) const
        { return normalSize; }
        Eyes normalEyes(void) const;
        int numLayersInVector(void) const
        { return layersInVector; }
        int minEyeDistance(void) { return minEyeDist; }
        int maxEyeAdjust(void) { return maxEyeAdj ? maxEyeAdj : 17; }

    private:
        int maxEyeAdj;
        int minEyeDist;
        //		QVector<EigenFaceVectorType> templateVectors;
        int maxAvgsInTemplate;
        QSize normalSize;
        int eyeDistance;
        int layersInVector;
        static DDT::Return staticReturn;
    }; // EigenFaceParameters


} // INDI
