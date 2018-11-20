#include "LPModel/terms/sqc/adapters/pointel/control/CPointelContribution.h"

using namespace LPModel::Terms::SquaredCurvature::Internal;

Pointel::PointelContribution Pointel::setContribution(const Parameters &prm,
                                                      const Constants &sqc)
{
    PointelContribution::UnaryMap um;
    PointelContribution::BinaryMap bm;
    PointelContribution::TernaryMap tm;

    BinaryHandle bh(bm,sqc);
    TernaryHandle th(tm,sqc);

    IteratorManager::run(prm,bh,th);


    return PointelContribution(um,bm,tm);
}