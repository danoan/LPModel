#include "LPModel/terms/sqc/CSqc.h"

using namespace LPModel::Terms;
using namespace LPModel::Terms::SquaredCurvature;

Term API::prepare(const Parameters &prm,
                  const Grid& grid,
                  double weight)
{
    Constants sqc = CConstants::setConstants(prm);
    return CTerm::setTerm(prm,grid,sqc,weight);

}
