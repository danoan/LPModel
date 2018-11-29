#include "LPModel/terms/sqc/adapters/linel/control/IteratorManager.h"

using namespace LPModel::Terms::SquaredCurvature::Internal::Linel;

void IteratorManager::run(const Parameters &prm,
                          const Grid& grid,
                          const BinaryCallback &bcbk,
                          const TernaryCallback &tcbk)
{
    DGtal::Z2i::DigitalSet temp(prm.odrModel.domain);
    DIPaCUS::Misc::DigitalBallIntersection DBIOptimization = prm.handle.intersectionComputer(prm.radius,
                                                                                             prm.odrModel.optRegion);

    KSpace kspace;
    kspace.init(prm.odrModel.domain.lowerBound(),prm.odrModel.domain.upperBound(),true);
    for(auto linelPtr=grid.linelMap.begin();linelPtr!=grid.linelMap.end();++linelPtr)
    {
        KSpace::SCell linel =  kspace.sCell( linelPtr->first,true);
        Point linelCoord = linel.preCell().coordinates;

        KSpace::SCells pointels = kspace.sLowerIncident(linel);

        for(auto pointelPtr=pointels.begin();pointelPtr!=pointels.end();++pointelPtr)
        {
            KSpace::SCell pointel = *pointelPtr;
            Point pointelCoord = pointel.preCell().coordinates;

            temp.clear();
            DBIOptimization(temp, pointelCoord);

            for (auto pit = temp.begin(); pit != temp.end(); ++pit)
            {
                bcbk(linelCoord,pointelCoord,*pit);

                auto npit = pit;
                ++npit;
                for(;npit!=temp.end();++npit)
                {
                    tcbk(linelCoord,pointelCoord,*pit,*npit);
                }
            }
        }

    }

}