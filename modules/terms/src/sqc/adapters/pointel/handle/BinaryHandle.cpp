#include "LPModel/terms/sqc/adapters/pointel/handle/BinaryHandle.h"

using namespace LPModel::Terms::SquaredCurvature::Internal::Pointel;

void BinaryHandle::operator()(const Point &pointel,
                              const Point &pixel)
{
    PointMultiIndex bi;
    bi << pointel << pixel;

    if(this->bm.find(bi)==this->bm.end()) this->bm[bi]=0;
    this->bm[bi] += this->sqc.unaryContribution.at(pointel);
}