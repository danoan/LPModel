#include "LPModel/terms/sqc/control/IteratorManager.h"

using namespace LPModel::Terms::SquaredCurvature;

void IteratorManager::run(const Parameters &prm, const BinaryCallback &bcbk, const TernaryCallback &tcbk)
{
    DGtal::Z2i::DigitalSet temp(prm.odrModel.domain);
    DIPaCUS::Misc::DigitalBallIntersection DBIOptimization = prm.handle.intersectionComputer(prm.radius,
                                                                                             prm.pixelOptRegion);


    for(auto pointelPtr=prm.odrModel.applicationRegion.begin();pointelPtr!=prm.odrModel.applicationRegion.end();++pointelPtr)
    {
        DGtal::Z2i::Point pointel = *pointelPtr;

        temp.clear();
        DBIOptimization(temp, pointel);

        for (auto pit = temp.begin(); pit != temp.end(); ++pit)
        {
            bcbk(pointel,*pit);

            auto npit = pit;
            ++npit;
            for(;npit!=temp.end();++npit)
            {
                tcbk(pointel,*pit,*npit);
            }
        }
    }
}