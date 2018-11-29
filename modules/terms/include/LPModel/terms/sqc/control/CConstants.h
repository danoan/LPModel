#ifndef LPMODEL_TERMS_SQC_CCONSTANTS_H
#define LPMODEL_TERMS_SQC_CCONSTANTS_H

#include <LPModel/terms/sqc/model/Constants.h>
#include <LPModel/initialization/model/Parameters.h>

namespace LPModel
{
    namespace Terms
    {
        namespace SquaredCurvature
        {
            namespace CConstants
            {
                typedef Initialization::Parameters Parameters;

                typedef DGtal::Z2i::DigitalSet DigitalSet;
                typedef DGtal::Z2i::Domain Domain;
                typedef DGtal::Z2i::Point Point;

                Constants setConstants(const Parameters &prm);
            }
        }
    }
}

#endif //LPMODEL_TERMS_SQC_CCONSTANTS_H
