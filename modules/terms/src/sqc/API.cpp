#include "LPModel/terms/sqc/API.h"

using namespace LPModel::Terms::SquaredCurvature;

Term API::prepare(const Parameters &prm,
                  const Grid& grid)
{
    Constants sqc = CConstants::setConstants(prm);
    return CTerm::setTerm(prm,grid,sqc);

}
