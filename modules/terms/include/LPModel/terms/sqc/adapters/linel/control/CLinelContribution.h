#ifndef LPMODEL_TERMS_SQC_CLINELCONTRIBUTION_H
#define LPMODEL_TERMS_SQC_CLINELCONTRIBUTION_H

#include <LPModel/initialization/model/Parameters.h>

#include <LPModel/terms/sqc/adapters/linel/control/IteratorManager.h>
#include <LPModel/terms/sqc/adapters/linel/model/LinelContribution.h>

#include <LPModel/terms/sqc/adapters/linel/handle/BinaryHandle.h>
#include <LPModel/terms/sqc/adapters/linel/handle/TernaryHandle.h>

namespace LPModel
{
    namespace Terms
    {
        namespace SquaredCurvature
        {
            namespace Internal
            {
                namespace Linel
                {
                    typedef Initialization::Parameters Parameters;
                    typedef Initialization::Grid Grid;

                    typedef Pointel::PointelContribution PointelContribution;

                    LinelContribution setContribution(const Parameters& prm,
                                                      const Grid& grid,
                                                      const PointelContribution& pctbr);
                }
            }
        }
    }
}

#endif //LPMODEL_TERMS_SQC_CLINELCONTRIBUTION_H
