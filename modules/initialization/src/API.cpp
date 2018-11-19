#include "LPModel/initialization/API.h"

using namespace LPModel::Initialization;

API::DigitalSet API::Internal::pixelOptRegion(const DigitalSet &ds)
{
    ODRInterpixels odrInterpixels(ODRModel::AC_PIXEL,
                                  ODRModel::CM_PIXEL,
                                  3,
                                  ODRModel::FourNeighborhood);

    ODRModel odrModel = odrInterpixels.createODR(ODRModel::OM_OriginalBoundary,
                                                 ODRModel::AM_AroundBoundary,
                                                 3,
                                                 ds);

    DigitalSet optRegion = odrModel.optRegion;
    optRegion.insert(odrModel.applicationRegion.begin(),odrModel.applicationRegion.end());

    return optRegion;
}

Parameters API::initParameters(const DigitalSet &originalDS)
{
    const Domain &domain = originalDS.domain();
    DigitalSet boundary(domain);
    DigitalSet optRegion(domain);

    ODRInterpixels odrInterpixels(ODRModel::AC_POINTEL,
                                  ODRModel::CM_PIXEL,
                                  3,
                                  ODRModel::FourNeighborhood);

    ODRModel odrModel = odrInterpixels.createODR(ODRModel::OM_OriginalBoundary,
                                                 ODRModel::AM_AroundBoundary,
                                                 3,
                                                 originalDS);


    DigitalSet pixelOptRegion = Internal::pixelOptRegion(originalDS);


    DigitalSet optRegionDS = odrModel.optRegion;
    optRegionDS.insert(odrModel.applicationRegion.begin(), odrModel.applicationRegion.end());


    InterpixelSpaceHandle* ish = (InterpixelSpaceHandle*) odrInterpixels.handle();

    return Parameters( ODRModel(odrModel.domain,
                                odrModel.original,
                                optRegionDS,
                                odrModel.trustFRG,
                                odrModel.trustBKG,
                                odrModel.applicationRegion,
                                odrModel.toImageCoordinates),
                       *ish,
                       pixelOptRegion );
}

Grid API::createGrid(const DigitalSet &ds)
{
    Grid grid(ds);
    assert(grid.pixelMap.size()==ds.size()+1);

    int boundaryLinels = Internal::boundaryLinels(ds);

    assert(grid.linelMap.size()==(ds.size()*2+boundaryLinels/2));
    assert(grid.edgeMap.size()==grid.linelMap.size()*2);

    return grid;
}

int API::Internal::boundaryLinels(const DigitalSet &ds)
{
    DigitalSet setOnPixelSpace(ds.domain());
    std::for_each(ds.begin(),ds.end(),[&setOnPixelSpace](KPoint p)mutable{ setOnPixelSpace.insert( (p-Point(1,1))/2 );});

    DigitalSet digitalBoundary(ds.domain());
    FourDigitalBoundary(digitalBoundary,setOnPixelSpace);

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

