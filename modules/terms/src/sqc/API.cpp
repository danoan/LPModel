#include "LPModel/terms/sqc/API.h"

using namespace LPModel::Terms::SquaredCurvature;

void API::prepare(const Parameters &prm,
                  const Grid& grid)
{
    Constants sqc = CConstants::setConstants(prm);
    Term sqTerm = CTerm::setTerm(prm,grid,sqc);

}
