#ifndef LPMODEL_TERMS_SQC_CLINELCONTRIBUTION_H
#define LPMODEL_TERMS_SQC_CLINELCONTRIBUTION_H

#include "LPModel/initialization/model/Parameters.h"
#include "LPModel/initialization/model/Grid.h"

#include "LPModel/terms/sqc/adapters/linel/model/LinelContribution.h"
#include "LPModel/terms/sqc/model/Constants.h"

#include "IteratorManager.h"
#include "LPModel/terms/sqc/adapters/linel/handle/BinaryHandle.h"
#include "LPModel/terms/sqc/adapters/linel/handle/TernaryHandle.h"

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
                    typedef DGtal::Z2i::Point Point;
                    typedef DGtal::Z2i::KSpace KSpace;
    
                    typedef LinelContribution::PointMultiIndex PointMultiIndex;
                    typedef std::map<PointMultiIndex,double> BinaryMap;
                    typedef std::map<PointMultiIndex,double> TernaryMap;
    
                    typedef Initialization::Parameters Parameters;
                    typedef SquaredCurvature::Constants Constants;
                    
    
                    LinelContribution setContribution(const Parameters &prm,
                                                      const Constants &sqc);
                    
                }
                
            }
            
        }
    }
}

#endif //LPMODEL_TERMS_SQC_CLINELCONTRIBUTION_H
