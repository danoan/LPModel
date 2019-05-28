#include <string>

#include <boost/filesystem.hpp>

#include <DGtal/helpers/StdDefs.h>

#include <LPModel/initialization/model/Parameters.h>
#include <LPModel/initialization/model/Grid.h>
#include <LPModel/initialization/API.h>
#include <LPModel/terms/model/Term.h>
#include <LPModel/terms/sqc/CSqc.h>
#include <LPModel/solassign/model/SolutionAssignment.h>
#include <LPModel/utils/dispUtils.h>
#include <LPModel/utils/compUtils.h>

#include "LPModel/nonlinopt/inputReader/InputData.h"
#include "LPModel/nonlinopt/activeSet/ActiveSetSolver.h"
#include "LPModel/nonlinopt/utils/utils.h"

using namespace DGtal::Z2i;
using namespace LPModel::NonLinOpt;

typedef LPModel::SolutionAssignment::SolutionVector SolutionVector;
typedef LPModel::SolutionAssignment::SolutionPairVector SolutionPairVector;

SolutionVector thresholdedSolution(const ActiveSetSolver::Vector& minSolVector,
                                   double threshold)
{
    SolutionVector sv;
    for(int i=0;i<minSolVector.rows();++i)
    {
        if( minSolVector.coeff(i) > threshold )sv.push_back(1);
        else sv.push_back(0);
    }

    return sv;
}


int main(int argc, char* argv[])
{
    InputData id = readInput(argc,argv);

    boost::filesystem::path p(id.outputPath);
    std::string outputFolder = p.remove_filename().string();
    boost::filesystem::create_directories(outputFolder);

    DigitalSet dsInput = LPModel::Utils::loadImageAsDigitalSet(id.pgmInputImage);


    LPModel::Initialization::Parameters prm = LPModel::Initialization::API::initParameters(dsInput,id.levels);
    LPModel::Initialization::Grid grid = LPModel::Initialization::API::createGrid(prm.odrModel.optRegion,
                                                                prm);

    LPModel::Terms::Term scTerm = LPModel::Terms::SquaredCurvature::API::prepare(prm,grid,id.sqWeight);

    SolutionPairVector ifeasSolution = NonLinOpt::Utils::findFeasibleSolution(outputFolder,scTerm,prm,grid);
    DigitalSet dsFeasibleSolution = LPModel::SolutionAssignment::digitalSetFromSolutionVector(ifeasSolution,prm,grid);
    LPModel::Utils::exportImageFromDigitalSet(dsFeasibleSolution,outputFolder + "/initial-feasible-solution.pgm");


    ActiveSetSolver solverAS(grid,scTerm);
    ActiveSetSolver::Vector v = solverAS.feasibleSolution(ifeasSolution);
    double optValue = solverAS.minimize(v,id.iterations);
    std::cout << "Optvalue: " << optValue << std::endl;


    SolutionVector sv = thresholdedSolution(solverAS.solutionVector,id.threshold);

    DigitalSet dsFinalSolution = LPModel::SolutionAssignment::digitalSetFromSolutionVector(sv,prm,grid);
    LPModel::Utils::exportImageFromDigitalSet(dsFinalSolution,id.outputPath);

    std::cout << "SQC Before: " << LPModel::Utils::sumSQC(dsFeasibleSolution) << std::endl;
    std::cout << "SQC After: " << LPModel::Utils::sumSQC(dsFinalSolution) << std::endl;


    return 0;
}