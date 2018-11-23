#include "DGtal/helpers/StdDefs.h"
#include "DGtal/io/boards/Board2D.h"

#include <LPModel/initialization/API.h>
#include <DGtal/io/writers/GenericWriter.h>
#include "LPModel/solassign/model/SolutionAssignment.h"

using namespace LPModel;

typedef DIPaCUS::Representation::Image2D Image2D;

int main(int argc, char* argv[])
{
    if(argc<5)
    {
        std::cerr << "Usage: parameter-file lp-solution-output-file output-file\n";
        exit(1);
    }

    std::string parameterFile = argv[1];
    std::string gridFile = argv[2];
    std::string lpOutputFilePath = argv[3];
    std::string outputFilePath = argv[4];

    std::cerr << "Reading Parameters Object\n";
    Initialization::Parameters prm( Initialization::API::readParametersFromFile(parameterFile) );

    std::cerr << "Generating Grid Object\n";
    Initialization::Grid* grid = Initialization::API::readGridFromFile(gridFile);


    std::cerr << "Generating Image solution\n";
    DGtal::Z2i::DigitalSet dsOutput = SolutionAssignment::readSolution(lpOutputFilePath,prm,*grid);


    Image2D imgOutput(dsOutput.domain());
    DIPaCUS::Representation::digitalSetToImage(imgOutput,dsOutput);
    DGtal::GenericWriter<Image2D>::exportFile(outputFilePath,imgOutput);

    return 0;

}