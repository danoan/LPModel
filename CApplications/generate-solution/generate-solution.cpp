#include "DGtal/helpers/StdDefs.h"
#include "DGtal/io/boards/Board2D.h"

#include <LPModel/initialization/API.h>
#include <DGtal/io/writers/GenericWriter.h>
#include "LPModel/solassign/model/SolutionAssignment.h"
#include "LPModel/terms/sqc/control/CConstants.h"
#include "LPModel/terms/sqc/model/Constants.h"

using namespace LPModel;

typedef DIPaCUS::Representation::Image2D Image2D;

int main(int argc, char* argv[])
{
    if(argc<6)
    {
        std::cerr << "Usage: pgmShapeImage optWidth gridObjectFile lpCplexFile outputFilePath\n";
        exit(1);
    }

    std::string pgmShapeImage = argv[1];
    int optWidth = std::atoi(argv[2]);
    std::string gridObjectFile = argv[3];
    std::string lpCplexFile = argv[4];
    std::string outputFilePath = argv[5];

    std::cerr << "Reading Parameters Object\n";
    Initialization::Parameters prm( Initialization::API::readParametersFromFile(pgmShapeImage,optWidth) );

    std::cerr << "Generating Grid Object\n";
    Initialization::Grid* grid = Initialization::API::readGridFromFile(gridObjectFile);


    std::cerr << "Generating Image solution\n";
    DGtal::Z2i::DigitalSet dsOutput = SolutionAssignment::readSolution(lpCplexFile,prm,*grid);


    Image2D imgOutput(dsOutput.domain());
    DIPaCUS::Representation::digitalSetToImage(imgOutput,dsOutput);
    DGtal::GenericWriter<Image2D>::exportFile(outputFilePath,imgOutput);

    return 0;

}