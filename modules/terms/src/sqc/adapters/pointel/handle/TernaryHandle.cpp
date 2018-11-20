#include "LPModel/terms/sqc/adapters/pointel/handle/TernaryHandle.h"

using namespace LPModel::Terms::SquaredCurvature::Internal::Pointel;

void TernaryHandle::operator()(const Point &pointel,
                               const Point &pixel1,
                               const Point &pixel2)
{
    PointMultiIndex ti;
    ti << pointel << pixel1 << pixel2;

    if(this->tm.find(ti)==this->tm.end()) this->tm[ti]=0;
    this->tm[ti]+=2;
}