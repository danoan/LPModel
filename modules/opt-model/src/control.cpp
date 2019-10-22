#include "LPModel/opt-model/control.h"

namespace LPModel
{
    namespace OptModel
    {
        DigitalSet level(const DTL2& dtL2, int lessThan, int greaterThan)
        {
            DigitalSet d(dtL2.domain());
            for(auto it=dtL2.domain().begin();it!=dtL2.domain().end();++it)
            {
                if(dtL2(*it)<=lessThan && dtL2(*it)>greaterThan) d.insert(*it);
            }

            return d;
        }

        OptModel createOptModel(const DigitalSet& original, int optWidth)
        {
            Domain domain = original.domain();

            DigitalSet compSquare(domain);
            compSquare.assignFromComplement(original);

            DTL2 internDT(domain, original, DGtal::Z2i::l2Metric);
            DTL2 externDT(domain,compSquare,DGtal::Z2i::l2Metric);


            DigitalSet optRegion(domain);
            DigitalSet trustFRG(domain);
            DigitalSet trustBKG(domain);

            DIPaCUS::Misc::digitalBoundary<DIPaCUS::Neighborhood::FourNeighborhoodPredicate>(optRegion,original,1);
            optRegion += level(internDT,optWidth,0);
            optRegion += level(externDT,optWidth,0);

            DIPaCUS::SetOperations::setDifference(trustFRG,original,optRegion);

            DigitalSet optPlusFRG = optRegion;
            optPlusFRG += trustFRG;
            trustBKG.assignFromComplement(optPlusFRG);

            KSpace kspace;
            kspace.init(domain.lowerBound(),domain.upperBound(),true);

            Domain interpixelDomain( domain.lowerBound()*2-Point(1,1),domain.upperBound()*2 + Point(1,1));
            DigitalSet appRegion(interpixelDomain);
            SCell pixelModel = kspace.sCell( Point(1,1), true);
            for(auto p:optRegion)
            {
                for(auto l:kspace.sLowerIncident(( kspace.sCell(p,pixelModel)) ) )
                {
                    appRegion.insert( kspace.sKCoords(l));
                }
            }

            return OptModel(original,optRegion,trustFRG,trustBKG,appRegion);
        }
    }
}