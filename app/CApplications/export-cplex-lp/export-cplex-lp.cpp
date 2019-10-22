#include <iostream>

#include "boost/filesystem.hpp"

#include <DGtal/io/writers/GenericWriter.h>

#include <LPModel/linearization/model/Linearization.h>
#include <LPModel/terms/model/Term.h>
#include <LPModel/terms/data/CData.h>
#include <LPModel/utils/dispUtils.h>

#include <LPModel/terms/API.h>
#include "LPModel/initialization/API.h"

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

int main(int argc, char* argv[])
{
    InputData in = readInput(argc,argv);
    DigitalSet ds = loadImageAsDigitalSet(in.pgmInputImage);
    

    Initialization::Parameters prm = Initialization::API::initParameters(ds,in.optRegionWidth);
    Initialization::Grid grid = Initialization::API::createGrid(prm.odrModel.optRegion,
                                                                prm);

    Terms::Term scTerm = SquaredCurvature::API::prepare(prm,grid,in.sqWeight);
    //Terms::Term dataTerm = DataFidelity::API::prepare(prm,grid,in.dataWeight);

    Terms::Term mergedTerm = scTerm;//Terms::API::merge(dataTerm,scTerm);


    typedef Linearization< Terms::Term::UIntMultiIndex,double > MyLinearization;

    unsigned long nextIndex = grid.pixelMap.size()-3+grid.edgeMap.size();
    MyLinearization linearization(nextIndex);

    switch(in.linearizationLevel)
    {
        case LPModel::LINEARIZATION_ALL_COUPLED:
        {
            linearization.linearize(mergedTerm.binaryMap);
            linearization.coupledLinearization(mergedTerm.ternaryMap);
            break;
        }
        case LPModel::LINEARIZATION_ALL_UNCOUPLED:
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


    LPWriter::writeLP(in.outputPath,prm,grid,mergedTerm.unaryMap,linearization,in.relaxationLevel);
    std::ofstream ofs(in.outputPath + "/inputData.txt");
    printInputData(ofs,in);


    return 0;
}