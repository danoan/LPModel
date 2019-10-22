#include "LPModel/terms/sqc/adapters/linel/control/IteratorManager.h"

using namespace LPModel::Terms::SquaredCurvature::Internal::Linel;

void IteratorManager::run(const Parameters &prm, const BinaryCallback &bcbk, const TernaryCallback &tcbk)
{
    DGtal::Z2i::DigitalSet temp(prm.odrModel.domain);
    DIPaCUS::Misc::DigitalBallIntersection DBIOptimization = prm.handle.intersectionComputer(prm.odrModel.optRegion);

    double bSum=0;
    double tSum=0;


    for(auto linelPtr=prm.odrModel.applicationRegion.begin();linelPtr!=prm.odrModel.applicationRegion.end();++linelPtr)
    {
        DGtal::Z2i::Point linel = *linelPtr;

        temp.clear();
        DBIOptimization(temp, linel);

        tSum += (temp.size()*(temp.size()-1))/2;
        bSum += temp.size();

        for (auto pit = temp.begin(); pit != temp.end(); ++pit)
        {
            bcbk(linel,*pit);

            
            for(auto npit = pit;npit!=temp.end();++npit)
            {
                tcbk(linel,*pit,*npit);
            }
        }
    }

}