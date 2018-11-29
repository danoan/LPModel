#include "DGtal/helpers/StdDefs.h"
#include "DGtal/io/boards/Board2D.h"

#include <LPModel/initialization/CData.h>
#include <DGtal/io/writers/GenericWriter.h>
#include "LPModel/solassign/model/SolutionAssignment.h"
#include "modules/terms/include/LPModel/terms/sqc/control/CConstants.h"
#include "modules/terms/include/LPModel/terms/sqc/model/Constants.h"

using namespace LPModel;

typedef DIPaCUS::Representation::Image2D Image2D;

int main(int argc, char* argv[])
{
    if(argc<6)
    {
        std::cerr << "Usage: parameter-file levels lp-solution-output-file output-file\n";
        exit(1);
    }

    std::string parameterFile = argv[1];
    int levels = std::atoi(argv[2]);
    std::string gridFile = argv[3];
    std::string lpOutputFilePath = argv[4];
    std::string outputFilePath = argv[5];

    std::cerr << "Reading Parameters Object\n";
    Initialization::Parameters prm( Initialization::API::readParametersFromFile(parameterFile,levels) );

    std::cerr << "Generating Grid Object\n";
    Initialization::Grid* grid = Initialization::API::readGridFromFile(gridFile);


    std::cerr << "Generating Image solution\n";
    DGtal::Z2i::DigitalSet dsOutput = SolutionAssignment::readSolution(lpOutputFilePath,prm,*grid);


    Image2D imgOutput(dsOutput.domain());
    DIPaCUS::Representation::digitalSetToImage(imgOutput,dsOutput);
    DGtal::GenericWriter<Image2D>::exportFile(outputFilePath,imgOutput);

    return 0;

}