#include "LPModel/initialization/API.h"

using namespace LPModel::Initialization;

ODRModel API::createOdrModel(const DigitalSet &originalDS)
{
    const Domain &domain = originalDS.domain();
    DigitalSet boundary(domain);
    DigitalSet optRegion(domain);

    ODRPixels odrPixels(ODRModel::AC_PIXEL,
                        ODRModel::CM_PIXEL,
                        3,
                        ODRModel::FourNeighborhood);

    ODRModel odrModel = odrPixels.createODR(ODRModel::OM_OriginalBoundary,
                                            ODRModel::AM_AroundBoundary,
                                            3,
                                            originalDS);

    DigitalSet optRegionDS = odrModel.optRegion;
    optRegionDS.insert(odrModel.applicationRegion.begin(), odrModel.applicationRegion.end());

    return ODRModel(odrModel.domain,
                    odrModel.original,
                    optRegionDS, odrModel.trustFRG,
                    odrModel.trustBKG,
                    odrModel.applicationRegion,
                    odrModel.toImageCoordinates);
}

Grid API::createGrid(const DigitalSet &ds)
{
    Grid grid(ds);
    assert(grid.pixelMap.size()==ds.size()+1);

    DigitalSet boundaryDS(ds.domain());
    FourDigitalBoundary(boundaryDS,ds);
    int boundaryLinels = Internal::uniqueLinels(boundaryDS);

    assert(grid.linelSet.size()==(ds.size())*2+boundaryLinels/2);
    assert(grid.edgeMap.size()==grid.linelSet.size()*2);

    return grid;
}

int API::Internal::uniqueLinels(const DigitalSet &digitalBoundary)
{
    std::set<DGtal::Z2i::Point> linelCoordSet;
    DGtal::Z2i::KSpace kspace;
    kspace.init(digitalBoundary.domain().lowerBound(),
                digitalBoundary.domain().upperBound(),
                true);

    DGtal::Z2i::SCell pixelModel = kspace.sCell( DGtal::Z2i::Point(1,1),true);
    for(auto it=digitalBoundary.begin();it!=digitalBoundary.end();++it)
    {
        DGtal::Z2i::SCell pixel = kspace.sCell(*it,pixelModel);
        DGtal::Z2i::SCells scells = kspace.sLowerIncident(pixel);
        for(auto s=scells.begin();s!=scells.end();++s)
        {
            if(linelCoordSet.find(s->preCell().coordinates)==linelCoordSet.end())
                linelCoordSet.insert(s->preCell().coordinates);
            else
                linelCoordSet.erase(s->preCell().coordinates);
        }

    }

    return linelCoordSet.size()/2;
}

