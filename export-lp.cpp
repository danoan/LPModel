#include <iostream>

#include <LPModel/linearization/model/Linearization.h>
#include <DGtal/io/writers/GenericWriter.h>

#include "LPModel/initialization/API.h"
#include "LPModel/initialization/Shapes.h"

#include "LPModel/terms/sqc/API.h"
#include "LPModel/constraints/ClosedAndConnected.h"
#include "LPModel/lpwriter/LPWriter.h"
#include "LPModel/objective/model/Objective.h"

#include "boost/filesystem.hpp"

using namespace LPModel;
using namespace LPModel::Terms;

typedef DIPaCUS::Representation::Image2D Image2D;
typedef DGtal::Z2i::DigitalSet DigitalSet;

typedef Linearization< SquaredCurvature::Term::UIntMultiIndex,double > MyLinearization;

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
             const MyLinearization& linearization)
{
    std::cerr << "Writing LP-Program at " << outputFilePath << "\n";

    boost::filesystem::path p(outputFilePath);
    boost::filesystem::create_directories(p.remove_filename());

    std::ofstream ofs(outputFilePath);
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
    if(argc<3)
    {
        std::cerr <<"Expected pgm-input-image output-path\n";
        exit(1);
    }

    std::string pgmInputImage = argv[1];
    std::string outputPath = argv[2];

    DigitalSet ds = loadImageAsDigitalSet(pgmInputImage);
    

    Initialization::Parameters prm = Initialization::API::initParameters(ds);
    Initialization::Grid grid = Initialization::API::createGrid(prm.odrModel.optRegion,
                                                                prm);
    SquaredCurvature::Term term = SquaredCurvature::API::prepare(prm,grid);


    unsigned long nextIndex = grid.pixelMap.size()+grid.linelMap.size()+grid.edgeMap.size();
    MyLinearization linearization(nextIndex);
    linearization.linearize(term.binaryMap);
    linearization.linearize(term.ternaryMap);

    std::string lpOutputFilePath = resolveLPOutputFilePath(outputPath,pgmInputImage);
    writeLP(lpOutputFilePath,prm,grid,linearization);

    saveObjects(outputPath,ds,grid);




    return 0;
}