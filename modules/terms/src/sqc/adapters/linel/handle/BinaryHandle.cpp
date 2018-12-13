#include "LPModel/terms/sqc/adapters/linel/handle/BinaryHandle.h"

using namespace LPModel::Terms::SquaredCurvature::Internal::Linel;

void BinaryHandle::operator()(const Point &linel,
                              const Point &pixel)
{
    PointMultiIndex bi;
    bi << linel << pixel;


    if(this->bm.find(bi)==this->bm.end()) this->bm[bi]=0;
    this->bm[bi] += this->sqc.unaryContribution.at(linel);
}