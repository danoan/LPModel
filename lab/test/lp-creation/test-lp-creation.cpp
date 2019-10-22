#include <DGtal/helpers/StdDefs.h>
#include <SCaBOliC/Core/model/ODRModel.h>
#include <SCaBOliC/Core//ODRPixels.h>
#include <LPModel/initialization/API.h>
#include <LPModel/utils/dispUtils.h>
#include <LPModel/terms/model/Term.h>
#include <LPModel/terms/sqc/CSqc.h>
#include "LPModel/lpwriter/LPWriter.h"


using namespace LPModel;
using namespace SCaBOliC::Core;
using namespace DGtal::Z2i;

std::string projectDir = PROJECT_DIR;
std::string outputFolder = projectDir + "/lab/test/lp-creation/output/test/lp-creation";

Initialization::Parameters initParameters()
{
    Domain domain( Point(0,0), Point(20,20) );

    DigitalSet original(domain);
    original.insert(Point(3,4));
    original.insert(Point(4,4));
    original.insert(Point(5,5));
    original.insert(Point(5,3));
    original.insert(Point(6,4));
    original.insert(Point(5,4));

    DGtal::Board2D board;
    board << original;
    board.saveEPS( (outputFolder + "/input-set.eps").c_str() );

    double radius = 5;
    int levels = 0;
    double gridStep = 1.0;

    ODRPixels odrLinels(radius,
                                  gridStep,
                                  levels,
                                  ODRModel::LevelDefinition::LD_CloserFromCenter,
                                  ODRModel::FourNeighborhood);

    ODRModel odrModel = odrLinels.createODR(original,ODRModel::OM_CorrectConvexities);



    DigitalSet extendedOptRegion = odrModel.optRegion;
    DigitalSet extendedAppRegion = Initialization::API::Internal::extendedAppRegion(odrModel.applicationRegion,extendedOptRegion);

    DigitalSet reducedTrustFrg(odrModel.trustFRG.domain());
    DIPaCUS::SetOperations::setDifference(reducedTrustFrg,odrModel.trustFRG,extendedOptRegion);

    PixelSpaceHandle* ish = (PixelSpaceHandle*) odrLinels.handle();

    return Initialization::Parameters( ODRModel(odrModel.domain,
                                                odrModel.original,
                                                extendedOptRegion,
                                                reducedTrustFrg,//odrModel.trustFRG,
                                                odrModel.trustBKG,
                                                extendedAppRegion,
                                                odrModel.toImageCoordinates),
                                       *ish,
                                       radius);

}


int main(int argc, char* argv[])
{
    boost::filesystem::create_directories(outputFolder);

    Initialization::Parameters prm = initParameters();
    Utils::exportODRModel(prm,outputFolder+"/odr-model.eps");

    Initialization::Grid grid = Initialization::API::createGrid(prm.odrModel.optRegion,
                                                                prm);

    Terms::Term scTerm = Terms::SquaredCurvature::API::prepare(prm,grid,1.0);
    Terms::Term mergedTerm = scTerm;//Terms::API::merge(dataTerm,scTerm);


    unsigned long nextIndex = grid.pixelMap.size()-3+grid.edgeMap.size();
    LPWriter::MyLinearization linearization(nextIndex);
    linearization.linearize(mergedTerm.binaryMap);
    linearization.linearize(mergedTerm.ternaryMap);

    std::string lpOutputFilePath = outputFolder+"/lp-output.lp";
    LPWriter::writeLP(lpOutputFilePath,prm,grid,mergedTerm.unaryMap,linearization,2);

    return 0;
}