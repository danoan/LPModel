#include "LPModel/terms/sqc/adapters/linel/control/CLinelContribution.h"

using namespace LPModel::Terms::SquaredCurvature::Internal;

Linel::LinelContribution Linel::setContribution(const Parameters &prm,
                                                const Constants &sqc)
{
    LinelContribution::UnaryMap um;
    LinelContribution::BinaryMap bm;
    LinelContribution::TernaryMap tm;

    BinaryHandle bh(bm,sqc);
    TernaryHandle th(tm,sqc);

    IteratorManager::run(prm,bh,th);


    return LinelContribution(um,bm,tm);
}