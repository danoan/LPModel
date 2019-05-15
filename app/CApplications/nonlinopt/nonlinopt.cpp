#include <iostream>

#include <LPModel/terms/model/Term.h>
#include <LPModel/terms/data/CData.h>
#include <LPModel/terms/API.h>
#include <LPModel/nonlinopt/Minimizer.h>
#include <LPModel/lpwriter/LPWriter.h>
#include <LPModel/nonlinopt/MinimizerAR.h>
#include "LPModel/terms/sqc/CSqc.h"

#include "LPModel/initialization/API.h"
#include "LPModel/initialization/Shapes.h"

#include "adept.h"
#include "LPModel/solassign/model/SolutionAssignment.h"

using namespace LPModel;
using namespace LPModel::Terms;

typedef DIPaCUS::Representation::Image2D Image2D;
typedef DGtal::Z2i::DigitalSet DigitalSet;

DigitalSet loadImageAsDigitalSet(const std::string& imageFilePath)
{
    std::cerr << "Load Image as DigitalSet\n";

    Image2D imgInput = DGtal::GenericReader<Image2D>::import(imageFilePath);
    DigitalSet ds(imgInput.domain());
    DIPaCUS::Representation::imageAsDigitalSet(ds,imgInput);

    return ds;
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

    boost::filesystem::path p(outputPath);
    std::string outputFolder = p.remove_filename().string();
    boost::filesystem::create_directories(outputFolder);

    DigitalSet ds = loadImageAsDigitalSet(pgmInputImage);


    Initialization::Parameters prm = Initialization::API::initParameters(ds,levels);
    Initialization::Grid grid = Initialization::API::createGrid(prm.odrModel.optRegion,
                                                                prm);

    Terms::Term scTerm = SquaredCurvature::API::prepare(prm,grid,sqWeight);
    Terms::Term mergedTerm = scTerm;//Terms::API::merge(dataTerm,scTerm);


    //Find a feasible solution
    LPWriter::MyLinearization linearization(0);
    LPWriter::writeLP(outputFolder + "/temp.lp",prm,grid,mergedTerm.unaryMap,linearization,2);

    SolutionAssignment::SolutionPairVector spv  = SolutionAssignment::solutionPairVector(outputFolder + "/temp.sol",
                                                                                         prm,
                                                                                         grid);
    NonLinOpt::MinimizerAR minimizer(grid,mergedTerm);
    NonLinOpt::MinimizerAR::Vector feasVector = minimizer.feasibleSolution(spv);

    double optValue = minimizer.minimize(feasVector,10);
    std::cout << "Optvalue: " << optValue << std::endl;

//    free(varVector);

    return 0;
}