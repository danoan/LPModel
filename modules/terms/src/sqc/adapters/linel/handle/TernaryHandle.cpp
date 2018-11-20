#include "LPModel/terms/sqc/adapters/linel/handle/TernaryHandle.h"

using namespace LPModel::Terms::SquaredCurvature::Internal::Linel;

void TernaryHandle::operator()(const Point &linel,
                               const Point &pointel,
                               const Point &pixel1,
                               const Point &pixel2)
{
    PointelContribution::PointMultiIndex pointelMI;
    pointelMI << pointel << pixel1 << pixel2;

    LinelContribution::PointMultiIndex linelMI;
    linelMI << linel << pixel1 << pixel2;

    if(this->tm.find(linelMI)==this->tm.end()) this->tm[linelMI]=0;
    this->tm[linelMI]+=this->pctbr.ternaryMap.at(pointelMI);
}