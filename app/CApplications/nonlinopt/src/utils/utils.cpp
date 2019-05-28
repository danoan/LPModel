#include "LPModel/nonlinopt/utils/utils.h"

namespace LPModel
{
    namespace NonLinOpt
    {
        namespace Utils
        {
            SolutionAssignment::SolutionPairVector cleansedVector(const SolutionPairVector& spv)
            {
                SolutionPairVector cleansed;
                for(auto it=spv.begin();it!=spv.end();++it)
                {
                    SolutionAssignment::Index index = it->first;
                    double v = it->second;

                    if(v<0) v = 0;
                    if(v>1) v = 1;

                    cleansed.push_back( SolutionAssignment::SolutionPair(index,v) );
                }

                return cleansed;
            }

            SolutionAssignment::SolutionPairVector findFeasibleSolution(const std::string& outputFolder,
                                                                        const Term& mergedTerm,
                                                                        const Parameters& prm,
                                                                        const Grid& grid)
            {
                //Find a feasible solution
                std::string lpOutputFile = outputFolder + "/temp.lp";
                std::string solutionFile = outputFolder + "/temp.sol";

                unsigned long nextIndex = grid.pixelMap.size()-3+grid.edgeMap.size();
                unsigned long numVars = nextIndex;
                LPWriter::MyLinearization linearization(nextIndex);

                linearization.linearize(mergedTerm.binaryMap);
                linearization.coupledLinearization(mergedTerm.ternaryMap);

                LPWriter::writeLP(lpOutputFile,prm,grid,mergedTerm.unaryMap,linearization,RelaxationLevel::RELAXATION_ALL);

                GLPK::glpk_solver(lpOutputFile,solutionFile);

                SolutionAssignment::SolutionPairVector spv  = SolutionAssignment::solutionPairVector(solutionFile,
                                                                                                     prm,
                                                                                                     grid);

                return cleansedVector(spv);
            }

        }
    }
}