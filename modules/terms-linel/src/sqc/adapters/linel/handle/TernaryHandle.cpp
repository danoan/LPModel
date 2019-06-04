#include "LPModel/terms/sqc/adapters/linel/handle/TernaryHandle.h"

using namespace LPModel::Terms::SquaredCurvature::Internal::Linel;


void TernaryHandle::operator()(const PointMultiIndex& mi)
{
    this->tm[mi] = sqc.pairwiseContribution.at(mi);
}