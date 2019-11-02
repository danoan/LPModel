#include <iostream>

#include <DGtal/io/writers/GenericWriter.h>

#include "boost/filesystem.hpp"

#include <LPModel/linearization/model/Linearization.h>

#include <LPModel/terms/model/Term.h>
#include <LPModel/terms/data/CData.h>
#include <LPModel/utils/dispUtils.h>
#include <LPModel/terms/API.h>

#include "LPModel/initialization/API.h"

#include "LPModel/terms/sqc/CSqc.h"
#include "LPModel/constraints/ClosedAndConnected.h"
#include "LPModel/lpwriter/LPWriter.h"

#include "InputData.h"
#include "InputReader.h"

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
    InputData in;
    in = readInput(argc,argv);

    std::cerr << "Preparing QP for image: " << in.pgmInputImage << "\n"
              << "with sq-weight=" << in.sqWeight << "; data-weight=" << in.dataWeight << "\n"
              << "with relaxation level=" << resolveRelaxationLevelName(in.relaxationLevel) << "\n"
              << "with linearization level=" << resolveLinearizationLevelName(in.linearizationLevel) << "\n"
              << "; and optimization region width of " << in.optRegionWidth << " pixels around the boundary ...\n";



    DigitalSet ds = loadImageAsDigitalSet(in.pgmInputImage);


    Initialization::Parameters prm = Initialization::API::initParameters(ds,in.optRegionWidth);
    Initialization::Grid* grid = Initialization::API::readGridFromFile(in.gridObjectFile);

    Terms::Term scTerm = SquaredCurvature::API::prepare(prm,*grid,in.sqWeight);
    //Terms::Term dataTerm = DataFidelity::API::prepare(prm,grid,in.dataWeight);

    Terms::Term mergedTerm = scTerm;//Terms::API::merge(dataTerm,scTerm);

    typedef Linearization< Terms::Term::UIntMultiIndex,double > MyLinearization;
    unsigned long nextIndex = grid->pixelMap.size()-3+grid->edgeMap.size(); //TODO: was + grid.linelMap.size()
    LPWriter::MyLinearization linearization(nextIndex);

    Terms::Term::BinaryMap partialLinearizationBM;

    switch(in.linearizationLevel)
    {
        case LPModel::LINEARIZATION_PIXEL_LINEL:
        {
            linearization.linearize(mergedTerm.binaryMap);
            linearization.coupledLinearization(mergedTerm.ternaryMap);
            break;
        }
        case LPModel::LINEARIZATION_PIXEL_PAIR:
        {
            MyLinearization::PartialLinearizationPair plp(Initialization::IVariable::Pixel,Initialization::IVariable::Pixel);
            partialLinearizationBM = linearization.partialLinearization(mergedTerm.ternaryMap,*grid,plp);
            break;
        }
        default:
        {
            std::cerr << "Invalid relaxation. Such relaxation leads to a linear or third order formulation.\n";
            exit(1);
        }
    }

    LPWriter::writeQP(in.outputPath,prm,*grid,mergedTerm.unaryMap,mergedTerm.binaryMap,partialLinearizationBM,linearization,in.relaxationLevel);



    return 0;
}