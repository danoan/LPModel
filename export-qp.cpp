#include <iostream>

#include <LPModel/linearization/model/Linearization.h>
#include <DGtal/io/writers/GenericWriter.h>
#include <LPModel/terms/model/Term.h>
#include <LPModel/terms/data/CData.h>
#include <LPModel/utils/dispUtils.h>
#include <LPModel/terms/API.h>

#include "LPModel/initialization/API.h"
#include "LPModel/initialization/Shapes.h"

#include "LPModel/terms/sqc/CSqc.h"
#include "LPModel/constraints/ClosedAndConnected.h"
#include "LPModel/lpwriter/LPWriter.h"

#include "boost/filesystem.hpp"

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
    if(argc<6)
    {
        std::cerr <<"Expected pgm-input-image levels sq-weight data-weight output-path\n";
        exit(1);
    }

    std::string pgmInputImage = argv[1];
    int levels = std::atoi(argv[2]);
    double sqWeight = std::atof( argv[3] );
    double dataWeight = std::atof( argv[4] );
    LPWriter::RelaxationLevel relLevel = (LPWriter::RelaxationLevel) std::atoi( argv[5] );
    std::string outputPath = argv[6];

    boost::filesystem::create_directories(outputPath);
    std::cerr << "Preparing LP for image: " << pgmInputImage << "\n"
              << "with sq-weight=" << sqWeight << "; data-weight=" << dataWeight << "\n"
              << "with relaxation level=" << relLevel
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
    LPWriter::MyLinearization linearization(nextIndex);
    Terms::Term::BinaryMap partialL = linearization.partialLinearization(mergedTerm.ternaryMap);

    std::string lpOutputFilePath = resolveLPOutputFilePath(outputPath,pgmInputImage);
    LPWriter::writeQP(lpOutputFilePath,prm,grid,mergedTerm.unaryMap,mergedTerm.binaryMap,partialL,linearization,relLevel);

    saveObjects(outputPath,ds,grid);




    return 0;
}