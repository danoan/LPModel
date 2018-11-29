#include <iostream>

#include <LPModel/linearization/model/Linearization.h>
#include <DGtal/io/writers/GenericWriter.h>
#include <LPModel/terms/model/Term.h>
#include <LPModel/terms/data/CData.h>
#include <LPModel/utils/dispUtils.h>
#include <LPModel/terms/API.h>

#include "LPModel/initialization/CData.h"
#include "LPModel/initialization/Shapes.h"

#include "LPModel/terms/sqc/CSqc.h"
#include "LPModel/constraints/ClosedAndConnected.h"
#include "LPModel/lpwriter/LPWriter.h"
#include "LPModel/objective/model/Objective.h"

#include "boost/filesystem.hpp"

using namespace LPModel;
using namespace LPModel::Terms;

typedef DIPaCUS::Representation::Image2D Image2D;
typedef DGtal::Z2i::DigitalSet DigitalSet;

typedef Linearization< Terms::Term::UIntMultiIndex,double > MyLinearization;

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

void writeLP(const std::string& outputFilePath,
             const Initialization::Parameters& prm,
             const Initialization::Grid& grid,
             const Terms::Term::UnaryMap& um,
             const MyLinearization& linearization)
{
    std::cerr << "Writing LP-Program at " << outputFilePath << "\n";

    boost::filesystem::path p(outputFilePath);
    boost::filesystem::create_directories(p.remove_filename());

    std::ofstream ofs(outputFilePath);
    ofs << "Minimize\n obj: ";

    Objective::writeObjective(ofs,um.begin(),um.end());
    Objective::writeObjective(ofs,linearization.begin(),linearization.end());


    ofs << "\nSubject To\n";
    int constraintNum=1;
    Objective::writeLinearizationConstraints(ofs,constraintNum,linearization.ubegin(),linearization.uend());


    Constraints::ClosedAndConnected::LinelConstraints lc;
    Constraints::ClosedAndConnected::closedConnectedContraints(lc,grid);
    LPWriter::writeConstraint(ofs,constraintNum,lc);

    ofs << "\nBounds\n";
    Objective::writeBounds(ofs,grid,linearization.begin(),linearization.end());
    ofs << "\nEnd";

    ofs.flush();
    ofs.close();
}

std::string resolveLPOutputFilePath(const std::string& outputFolder,
                                    const std::string& imageFilePath)
{
    boost::filesystem::path p(imageFilePath);
    return outputFolder + "/" + p.stem().string() + ".lp";
}

int main(int argc, char* argv[])
{
    if(argc<6)
    {
        std::cerr <<"Expected pgm-input-image levels sq-weight data-weight output-path\n";
        exit(1);
    }

    std::string pgmInputImage = argv[1];
    int levels = std::atoi(argv[2]);
    double sqWeight = std::atof( argv[3] );
    double dataWeight = std::atof( argv[4] );
    std::string outputPath = argv[5];

    std::cerr << "Preparing LP for image: " << pgmInputImage << "\n"
              << "with sq-weight=" << sqWeight << "; data-weight=" << dataWeight
              << "; and " << levels << " levels...\n";

    DigitalSet ds = loadImageAsDigitalSet(pgmInputImage);
    

    Initialization::Parameters prm = Initialization::API::initParameters(ds,levels);
    Utils::exportODRModel(prm,outputPath+"/odr-model.eps");

    Initialization::Grid grid = Initialization::API::createGrid(prm.odrModel.optRegion,
                                                                prm);

    Terms::Term scTerm = SquaredCurvature::API::prepare(prm,grid,sqWeight);
    //Terms::Term dataTerm = DataFidelity::API::prepare(prm,grid,dataWeight);

    Terms::Term mergedTerm = scTerm;//Terms::API::merge(dataTerm,scTerm);



    unsigned long nextIndex = grid.pixelMap.size()+grid.linelMap.size()+grid.edgeMap.size();
    MyLinearization linearization(nextIndex);
    linearization.linearize(mergedTerm.binaryMap);
    linearization.linearize(mergedTerm.ternaryMap);

    std::string lpOutputFilePath = resolveLPOutputFilePath(outputPath,pgmInputImage);
    writeLP(lpOutputFilePath,prm,grid,mergedTerm.unaryMap,linearization);

    saveObjects(outputPath,ds,grid);




    return 0;
}