#include <DGtal/helpers/StdDefs.h>
#include <LPModel/initialization/CData.h>
#include "LPModel/solassign/model/SolutionAssignment.h"

/*
 * Expected execution flow:
 *
 * Execute LP Model for three different instances of the
 * square image in input-images. The script that runs
 * the instance is:
 *
 *      lp-solve-scripts/gurobi/test-sqTerm
 *
 * The parameter levels varies among 1 and 3.
 *
 * The solution files are expected to be output in
 *
 *      output/lab/test-levels/
 *
 * The test-levels app is concerned to generate the
 * solution images and test its size accordingly
 * to expected solution images.
 */

using namespace LPModel;

unsigned long generateSolution(const std::string solutionFilesPath, int levels)
{
    std::string parameterFile = solutionFilesPath + "/prm.pgm";
    std::cerr << "Reading Parameters Object\n";
    Initialization::Parameters prm( Initialization::API::readParametersFromFile(parameterFile,levels) );

    std::string gridFile = solutionFilesPath + "/grid.obj";
    std::cerr << "Generating Grid Object\n";
    Initialization::Grid* grid = Initialization::API::readGridFromFile(gridFile);


    std::string lpOutputFilePath = solutionFilesPath + "/square.sol";
    std::cerr << "Generating Image solution\n";
    DGtal::Z2i::DigitalSet dsOutput = SolutionAssignment::readSolution(lpOutputFilePath,prm,*grid);


    return dsOutput.size();
}

int main(int argc, char* argv[])
{
    if(argc<2)
    {
        std::cerr << "Expected solution-files-path\n";
        exit(1);
    }

    std::string solutionFilesPath=argv[1];

    std::cerr << "Test one level...\n";
    assert( generateSolution(solutionFilesPath+"/l1",1)==165 );
    std::cerr << "Passed\nTest two levels...\n";
    assert( generateSolution(solutionFilesPath+"/l2",2)==109 );
    std::cerr << "Passed\nTest three levels...\n";
    assert( generateSolution(solutionFilesPath+"/l3",3)==69 );
    std::cerr << "Passed\n";

    return 0;
}