#include "LPModel/terms/sqc/adapters/linel/handle/BinaryHandle.h"

using namespace LPModel::Terms::SquaredCurvature::Internal::Linel;

void BinaryHandle::operator()(const PointMultiIndex& mi)
{
    this->bm[mi] = this->sqc.unaryContribution.at(mi);
}