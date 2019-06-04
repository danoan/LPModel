#ifndef LPMODEL_TERMSLINEL_SQC_API_H
#define LPMODEL_TERMSLINEL_SQC_API_H

#include "LPModel/initialization/model/Parameters.h"
#include "LPModel/initialization/model/Grid.h"

#include "LPModel/terms/sqc/control/CConstants.h"
#include <LPModel/terms/sqc/control/CTerm.h>

namespace LPModel
{
    namespace Terms
    {
        namespace SquaredCurvature
        {
            namespace API
            {
                typedef Initialization::Parameters Parameters;
                typedef Initialization::Grid Grid;


                Term prepare(const Parameters& prm,
                             const Grid& grid,
                             double weight);
            }
        }
    }
}

#endif //LPMODEL_TERMS_SQC_API_H
