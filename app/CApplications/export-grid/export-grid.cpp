#include "boost/filesystem.hpp"

#include "DIPaCUS/base/Representation.h"

#include "LPModel/initialization/API.h"
#include "LPModel/initialization/model/Grid.h"
#include "LPModel/initialization/model/Parameters.h"

#include "InputReader.h"

typedef DIPaCUS::Representation::Image2D Image2D;
typedef DIPaCUS::Representation::DigitalSet DigitalSet;

using namespace LPModel;

DigitalSet loadImageAsDigitalSet(const std::string& imageFilePath)
{
    std::cerr << "Load Image as DigitalSet\n";

    Image2D imgInput = DGtal::GenericReader<Image2D>::import(imageFilePath);
    DigitalSet ds(imgInput.domain());
    DIPaCUS::Representation::imageAsDigitalSet(ds,imgInput);

    return ds;
}

void saveObjects(const std::string& outputFilePath,
                 const DigitalSet& dsOriginal,
                 const Initialization::Grid& grid)
{
    std::cerr << "Saving Grid Object at: " << outputFilePath << "\n";
    
    boost::filesystem::path p(outputFilePath);
    boost::filesystem::create_directories(p.remove_filename());
    
    Initialization::API::save(grid,outputFilePath);
}

int main(int argc, char* argv[])
{
    InputData in = readInput(argc,argv);

    std::cerr << "Preparing Grid Object for image: " << in.pgmInputImage << "\n"
              << "with and optimization region width of " << in.optRegionWidth << " ...\n";


    DigitalSet ds = loadImageAsDigitalSet(in.pgmInputImage);


    Initialization::Parameters prm = Initialization::API::initParameters(ds,in.optRegionWidth);
    Initialization::Grid grid = Initialization::API::createGrid(prm.odrModel.optRegion,
                                                                prm);

    saveObjects(in.outputPath,ds,grid);

}
