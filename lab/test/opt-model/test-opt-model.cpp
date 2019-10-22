#include <boost/filesystem.hpp>

#include <DGtal/helpers/StdDefs.h>
#include <DIPaCUS/base/Shapes.h>

#include "LPModel/opt-model/OptModel.h"
#include "LPModel/opt-model/control.h"
#include "LPModel/opt-model/utils.h"

using namespace LPModel::OptModel;

int main(int argc, char* argv[])
{
    int optWidth=1;

    std::string outputPath = argv[1];
    boost::filesystem::path p(outputPath);
    boost::filesystem::create_directories(p.remove_filename());

    DGtal::Z2i::DigitalSet square = DIPaCUS::Shapes::square();

    OptModel odr = createOptModel(square,optWidth);
    displayODR(odr,outputPath);

    return 0;
}