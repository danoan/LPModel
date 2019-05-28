#ifndef LPMODEL_NONLINOPT_UTILS_H
#define LPMODEL_NONLINOPT_UTILS_H

#include <LPModel/lpwriter/LPWriter.h>

#include <LPModel/initialization/model/Parameters.h>
#include <LPModel/initialization/model/Grid.h>
#include <LPModel/terms/model/Term.h>
#include <LPModel/solassign/model/SolutionAssignment.h>

#include "LPModel/nonlinopt/glpk/glpk.h"

namespace LPModel
{
    namespace NonLinOpt
    {
        namespace Utils
        {
            typedef SolutionAssignment::SolutionPairVector SolutionPairVector;
            typedef Initialization::Parameters Parameters;
            typedef Initialization::Grid Grid;
            typedef Terms::Term Term;

            SolutionAssignment::SolutionPairVector cleansedVector(const SolutionPairVector& spv);

            SolutionAssignment::SolutionPairVector findFeasibleSolution(const std::string& outputFolder,
                                                                        const Term& mergedTerm,
                                                                        const Parameters& prm,
                                                                        const Grid& grid);
        }
    }
}

#endif //LPMODEL_NONLINOPT_UTILS_H
