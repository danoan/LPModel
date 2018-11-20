#include "LPModel/terms/sqc/adapters/linel/control/CLinelContribution.h"

using namespace LPModel::Terms::SquaredCurvature::Internal;

Linel::LinelContribution Linel::setContribution(const Parameters& prm,
                                                const Grid& grid,
                                                const PointelContribution& pctbr)
{
    LinelContribution::UnaryMap um;
    LinelContribution::BinaryMap bm;
    LinelContribution::TernaryMap tm;

    BinaryHandle bh(bm,pctbr);
    TernaryHandle th(tm,pctbr);

    IteratorManager::run(prm,grid,bh,th);

    return LinelContribution (um,bm,tm);
}