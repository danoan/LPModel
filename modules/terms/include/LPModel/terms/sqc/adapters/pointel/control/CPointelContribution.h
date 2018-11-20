#ifndef LPMODEL_CPOINTELCONTRIBUTION_H
#define LPMODEL_CPOINTELCONTRIBUTION_H

#include "LPModel/initialization/model/Parameters.h"
#include "LPModel/initialization/model/Grid.h"

#include "LPModel/terms/sqc/adapters/pointel/model/PointelContribution.h"
#include "LPModel/terms/sqc/model/Constants.h"

#include "IteratorManager.h"
#include "LPModel/terms/sqc/adapters/pointel/handle/BinaryHandle.h"
#include "LPModel/terms/sqc/adapters/pointel/handle/TernaryHandle.h"

namespace LPModel
{
    namespace Terms
    {
        namespace SquaredCurvature
        {
            namespace Internal
            {
                namespace Pointel
                {
                    typedef DGtal::Z2i::Point Point;
                    typedef DGtal::Z2i::KSpace KSpace;
    
                    typedef PointelContribution::PointMultiIndex PointMultiIndex;
                    typedef std::map<PointMultiIndex,double> BinaryMap;
                    typedef std::map<PointMultiIndex,double> TernaryMap;
    
                    typedef Initialization::Parameters Parameters;
                    typedef SquaredCurvature::Constants Constants;
                    
    
                    PointelContribution setContribution(const Parameters &prm,
                                                        const Constants &sqc);
                    
                }
                
            }
            
        }
    }
}

#endif //LPMODEL_CPOINTELCONTRIBUTION_H
