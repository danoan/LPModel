#include <string>
#include <DIPaCUS/base/Representation.h>
#include <LPModel/initialization/API.h>

using namespace LPModel;

typedef DIPaCUS::Representation::Image2D Image2D;
typedef DGtal::Z2i::DigitalSet DigitalSet;

namespace LPModel{ namespace Initialization{
    Parameters API::initParameters(const DigitalSet &originalDS,bool evenIteration)
    {
        const Domain &domain = originalDS.domain();
        DigitalSet boundary(domain);
        DigitalSet optRegion(domain);

        ODRInterpixels odrInterpixels(ODRModel::AC_POINTEL,
                                      ODRModel::CM_PIXEL,
                                      1,
                                      ODRModel::FourNeighborhood,
                                      evenIteration);

        ODRModel odrModel = odrInterpixels.createODR(ODRModel::OM_OriginalBoundary,
                                                     ODRModel::AM_AroundBoundary,
                                                     3,
                                                     originalDS);



        DigitalSet extendedOptRegion = Internal::extendedOptRegion(odrModel);
        DigitalSet extendedAppRegion = Internal::extendedAppRegion(odrModel);


        InterpixelSpaceHandle* ish = (InterpixelSpaceHandle*) odrInterpixels.handle();

        return Parameters( ODRModel(odrModel.domain,
                                    odrModel.original,
                                    extendedOptRegion,
                                    odrModel.trustFRG,
                                    odrModel.trustBKG,
                                    extendedAppRegion,
                                    odrModel.toImageCoordinates),
                           *ish );
    }

    Parameters API::readParametersFromFile(const std::string &inputFile, bool fixedEvenIteration)
    {
        typedef DIPaCUS::Representation::Image2D Image2D;
        Image2D imgIn = DGtal::GenericReader<Image2D>::import(inputFile);
        DigitalSet ds(imgIn.domain());
        DIPaCUS::Representation::imageAsDigitalSet(ds,imgIn);

        return initParameters(ds,fixedEvenIteration);
    }
}}


DigitalSet loadImageAsDigitalSet(const std::string& imageFilePath)
{
    std::cerr << "Load Image as DigitalSet\n";

    Image2D imgInput = DGtal::GenericReader<Image2D>::import(imageFilePath);
    DigitalSet ds(imgInput.domain());
    DIPaCUS::Representation::imageAsDigitalSet(ds,imgInput);

    return ds;
}

void saveObjects(const std::string& outputPath,
                 const DigitalSet& dsOriginal,
                 const Initialization::Grid& grid)
{
    std::cerr << "Saving Objects\n";

    Initialization::API::save(dsOriginal,outputPath+"/prm.pgm");
    Initialization::API::save(grid,outputPath+"/grid.obj");
}

bool compareParameters(const Initialization::Parameters& prm1, const Initialization::Parameters& prm2)
{
    bool r = prm1.odrModel.optRegion.size()==prm2.odrModel.optRegion.size()
           && prm1.odrModel.domain.lowerBound()==prm2.odrModel.domain.lowerBound()
           && prm1.odrModel.domain.upperBound()==prm2.odrModel.domain.upperBound()
           && prm1.odrModel.original.size()==prm2.odrModel.original.size()
           && prm1.odrModel.trustBKG.size()==prm2.odrModel.trustBKG.size()
           && prm1.odrModel.trustFRG.size()==prm2.odrModel.trustFRG.size()
           && prm1.odrModel.applicationRegion.size()==prm2.odrModel.applicationRegion.size();

    r = r
        && prm1.dataWeight==prm2.dataWeight
        && prm1.squaredCurvatureWeight==prm2.squaredCurvatureWeight
        && prm1.radius==prm2.radius;

    return r;
}

bool compareGrid(const Initialization::Grid& g1, const Initialization::Grid& g2)
{
    bool r = g1.pixelMap.size()==g2.pixelMap.size()
           && g1.edgeMap.size()==g2.edgeMap.size()
           && g1.linelMap.size()==g2.linelMap.size();


    return r;
}

int main(int argc, char* argv[])
{
    if(argc<3)
    {
        std::cerr <<"Expected pgm-input-image output-path\n";
        exit(1);
    }

    std::string pgmInputImage = argv[1];
    std::string outputPath = argv[2];

    DigitalSet ds = loadImageAsDigitalSet(pgmInputImage);


    Initialization::Parameters prm = Initialization::API::initParameters(ds,true);
    Initialization::Grid grid = Initialization::API::createGrid(prm.odrModel.optRegion,
                                                                prm);

    saveObjects(outputPath,ds,grid);

    Initialization::Parameters readParameters = Initialization::API::readParametersFromFile(outputPath + "/prm.pgm",true);
    Initialization::Grid* readGrid = Initialization::API::readGridFromFile(outputPath + "/grid.obj");


    assert( compareParameters(prm,readParameters) );
    assert( compareGrid(grid,*readGrid) );

    delete readGrid;

    return 0;
}