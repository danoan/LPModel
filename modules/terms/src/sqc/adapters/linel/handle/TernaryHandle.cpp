#include "LPModel/terms/sqc/adapters/linel/handle/TernaryHandle.h"

using namespace LPModel::Terms::SquaredCurvature::Internal::Linel;

void TernaryHandle::operator()(const Point &linel,
                               const Point &pixel1,
                               const Point &pixel2)
{
    PointMultiIndex ti;
    ti << linel << pixel1 << pixel2;

    if(this->tm.find(ti)==this->tm.end()) this->tm[ti]=0;

    if(pixel1==pixel2)
    {
        this->tm[ti]+=1;
    }else
    {
        this->tm[ti]+=2;
    }

}