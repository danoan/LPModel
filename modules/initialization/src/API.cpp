#include "LPModel/initialization/API.h"

using namespace LPModel::Initialization;

API::DigitalSet API::Internal::extendedOptRegion(const ODRModel &odrModel)
{
    typedef DGtal::Z2i::Point Point;

    Point neighPixel[4] = {Point(1,0),Point(0,1),Point(-1,0),Point(0,-1)};
    std::map<Point,int> incidence;

    DigitalSet extOptRegion = odrModel.optRegion;
    for(auto it=odrModel.applicationRegion.begin();it!=odrModel.applicationRegion.end();++it)
    {
        Point p = *it;
        for(int i=0;i<4;++i)
        {
            Point np = *it+neighPixel[i];
            if( !(np(0)%2==1 && np(1)%2==1) ) continue; //If is not a pixel
            if(incidence.find(np)==incidence.end()) incidence[np]=0;
            incidence[np]+=1;
        }
    }

    for(auto it=incidence.begin();it!=incidence.end();++it)
    {
        if(it->second>=2) extOptRegion.insert(it->first);
    }

    return extOptRegion;
}

API::DigitalSet API::Internal::extendedAppRegion(const DigitalSet& appRegion, const DigitalSet& optRegion)
{
    typedef DGtal::Z2i::Point Point;
    Point neighPointel[4] = {Point(1,0),Point(0,1),Point(-1,0),Point(0,-1) };

    DigitalSet extAppRegion = appRegion;
    for(auto it=optRegion.begin();it!=optRegion.end();++it)
    {
        for(int i=0;i<4;++i)
        {
            Point np = *it + neighPointel[i];
            extAppRegion.insert(np);
        }
    }

    return extAppRegion;
}

bool API::Internal::isConnected(const DigitalSet& ds)
{
    DigitalSet component(ds.domain());
    Point neigh[4] = {Point(1,0),Point(0,1),Point(-1,0),Point(0,-1)};

    std::stack<Point> s;
    s.push(*ds.begin());
    while(!s.empty())
    {
        Point p = s.top();
        s.pop();

        if(component(p)) continue;
        if(!ds(p)) continue;

        component.insert(p);

        for(int i=0;i<4;++i)
        {
            Point np = p + neigh[i];
            s.push(np);
        }
    }

    return component.size()==ds.size();
}

bool API::Internal::consistentGrid(const Parameters& prm,
                                   const Grid& grid)
{
    DGtal::Z2i::KSpace kspace;
    kspace.init(prm.odrModel.domain.lowerBound(),prm.odrModel.domain.upperBound(),true);

    DigitalSet gridSet(prm.odrModel.domain);
    for(auto it=grid.linelMap.begin();it!=grid.linelMap.end();++it)
    {
        gridSet.insert(it->first);
    }

    for(auto it=grid.pixelMap.begin();it!=grid.pixelMap.end();++it)
    {
        if(it->second.ct!=Pixel::CellType::Variable) continue;
        gridSet.insert(it->first);
    }

    gridSet.insert(prm.odrModel.applicationRegion.begin(),prm.odrModel.applicationRegion.end());

    return isConnected(gridSet);
};

Parameters API::initParameters(const DigitalSet &originalDS, int levels)
{
    const Domain &domain = originalDS.domain();
    DigitalSet boundary(domain);
    DigitalSet optRegion(domain);

    ODRInterpixels odrInterpixels(ODRModel::AC_LINEL,
                                  ODRModel::CM_PIXEL,
                                  levels,
                                  ODRModel::LevelDefinition::LD_CloserFromCenter,
                                  ODRModel::FourNeighborhood,
                                  true);

    ODRModel odrModel = odrInterpixels.createODR(ODRModel::OM_OriginalBoundary,
                                                 ODRModel::AM_AroundIntern,
                                                 3,
                                                 originalDS);



    DigitalSet extendedOptRegion = Internal::extendedOptRegion(odrModel);
    DigitalSet extendedAppRegion = Internal::extendedAppRegion(odrModel.applicationRegion,extendedOptRegion);
    DigitalSet reducedTrustFrg(odrModel.trustFRG.domain());
    DIPaCUS::SetOperations::setDifference(reducedTrustFrg,odrModel.trustFRG,extendedOptRegion);


    InterpixelSpaceHandle* ish = (InterpixelSpaceHandle*) odrInterpixels.handle();

    return Parameters( ODRModel(odrModel.domain,
                                odrModel.original,
                                extendedOptRegion,
                                reducedTrustFrg,//odrModel.trustFRG,
                                odrModel.trustBKG,
                                extendedAppRegion,
                                odrModel.toImageCoordinates),
                       *ish );
}

Grid API::createGrid(const DigitalSet &ds,
                     const Parameters& prm)
{
    Grid grid(ds,prm.odrModel.trustFRG);
    assert(grid.pixelMap.size()==ds.size()+3);

    int boundaryLinels = Internal::boundaryLinels(ds);

    //assert(grid.linelMap.size()==(ds.size()*2+boundaryLinels/2));//TODO:: It is not the good assertion.
    assert(grid.edgeMap.size()==grid.linelMap.size()*2);

    assert(Internal::consistentGrid(prm,grid));

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

void API::save(const DigitalSet& dsOriginal, const std::string& outputFile)
{
    typedef DIPaCUS::Representation::Image2D Image2D;

    Image2D imgPRM( dsOriginal.domain() );
    DIPaCUS::Representation::digitalSetToImage(imgPRM,dsOriginal);
    DGtal::GenericWriter<Image2D>::exportFile(outputFile,imgPRM);
}

Parameters API::readParametersFromFile(const std::string &inputFile, int levels)
{
    typedef DIPaCUS::Representation::Image2D Image2D;
    Image2D imgIn = DGtal::GenericReader<Image2D>::import(inputFile);
    DigitalSet ds(imgIn.domain());
    DIPaCUS::Representation::imageAsDigitalSet(ds,imgIn);

    return initParameters(ds,levels);
}

void API::save(const Grid &grid, const std::string &outputFile)
{
    std::ofstream ofs(outputFile,std::ios_base::out);
    ofs << grid;

    ofs.flush();
    ofs.close();
}

Grid* API::readGridFromFile(const std::string &inputFile)
{
    std::ifstream ifs(inputFile);

    Grid* grid;
    ifs >> &grid;

    return grid;
}

