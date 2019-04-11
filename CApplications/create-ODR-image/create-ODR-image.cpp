#include "boost/filesystem.hpp"

#include "DIPaCUS/base/Representation.h"
#include "SCaBOliC/Core/display.h"

#include "LPModel/initialization/API.h"

#include "InputData.h"
#include "InputReader.h"

typedef DIPaCUS::Representation::DigitalSet DigitalSet;
typedef DIPaCUS::Representation::Image2D Image2D;

using namespace LPModel;

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

    SCaBOliC::Core::Display::DisplayODR(prm.odrModel,in.outputPath);
    return 0;
}