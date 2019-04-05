#include <iostream>

#include "boost/filesystem.hpp"

#include <DGtal/io/writers/GenericWriter.h>

#include <LPModel/linearization/model/Linearization.h>
#include <LPModel/terms/model/Term.h>
#include <LPModel/terms/data/CData.h>
#include <LPModel/utils/dispUtils.h>

#include <LPModel/terms/API.h>
#include "LPModel/initialization/API.h"

#include "LPModel/initialization/Shapes.h"
#include "LPModel/terms/sqc/CSqc.h"
#include "LPModel/constraints/ClosedAndConnected.h"

#include "LPModel/lpwriter/LPWriter.h"

#include "InputReader.h"
#include "InputData.h"

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

void saveObjects(const std::string& outputPath,
                 const DigitalSet& dsOriginal,
                 const Initialization::Grid& grid)
{
    std::cerr << "Saving Objects\n";

    Initialization::API::save(dsOriginal,outputPath+"/prm.pgm");
    Initialization::API::save(grid,outputPath+"/grid.obj");
}

std::string resolveLPOutputFilePath(const std::string& outputFolder,
                                    const std::string& imageFilePath)
{
    boost::filesystem::path p(imageFilePath);
    return outputFolder + "/" + p.stem().string() + ".lp";
}

int main(int argc, char* argv[])
{

    InputData in = readInput(argc,argv);

    boost::filesystem::create_directories(in.outputPath);
    std::cerr << "Preparing LP for image: " << in.pgmInputImage << "\n"
              << "with sq-weight=" << in.sqWeight << "; data-weight=" << in.dataWeight << "\n"
              << "with relaxation level=" << resolveRelaxationLevelName(in.relaxationLevel) << "\n"
              << "with linearization level=" << resolveLinearizationLevelName(in.linearizationLevel) << "\n"
              << "; and optimization region width of " << in.optRegionWidth << " ...\n";



    DigitalSet ds = loadImageAsDigitalSet(in.pgmInputImage);
    

    Initialization::Parameters prm = Initialization::API::initParameters(ds,in.optRegionWidth);
    Utils::exportODRModel(prm,in.outputPath+"/odr-model.eps");


    Initialization::Grid grid = Initialization::API::createGrid(prm.odrModel.optRegion,
                                                                prm);

    Terms::Term scTerm = SquaredCurvature::API::prepare(prm,grid,in.sqWeight);
    //Terms::Term dataTerm = DataFidelity::API::prepare(prm,grid,in.dataWeight);

    Terms::Term mergedTerm = scTerm;//Terms::API::merge(dataTerm,scTerm);


    typedef Linearization< Terms::Term::UIntMultiIndex,double > MyLinearization;

    unsigned long nextIndex = grid.pixelMap.size()+grid.linelMap.size()+grid.edgeMap.size();
    MyLinearization linearization(nextIndex);

    switch(in.linearizationLevel)
    {
        case InputData::LINEARIZATION_ALL_COUPLED:
        {
            linearization.linearize(mergedTerm.binaryMap);
            linearization.coupledLinearization(mergedTerm.ternaryMap);
            break;
        }
        case InputData::LINEARIZATION_ALL_UNCOUPLED:
        {
            linearization.linearize(mergedTerm.binaryMap);
            linearization.linearize(mergedTerm.ternaryMap);
            break;
        }
        default:
        {
            std::cerr << "Invalid relaxation. Such relaxation leads to a quadratic or third order formulation.\n";
            exit(1);
        }
    }


    std::string lpOutputFilePath = resolveLPOutputFilePath(in.outputPath,in.pgmInputImage);
    LPWriter::writeLP(lpOutputFilePath,prm,grid,mergedTerm.unaryMap,linearization,in.relaxationLevel);

    saveObjects(in.outputPath,ds,grid);


    return 0;
}